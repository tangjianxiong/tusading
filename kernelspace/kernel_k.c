#include <linux/init.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <net/netlink.h>
#include <net/sock.h>
#include <linux/debugfs.h>
#define NETLINK_TEST (25)
#define PID_A (100)
#define PID_B (101)
#define PID_C (102)
static dev_t devId;
static struct class *cls = NULL;
struct sock *nl_sk = NULL;
static u32 normal_num;
static u32 illegal_num;
static struct dentry *root_d;
static int getpid(char a)
{
    int res;
    switch (a)
    {

    case 'a':
        res = PID_A;
        break;
    case 'b':
        res = PID_B;
        break;

    case 'c':
        res = PID_C;
        break;

    default:
        res = -1;
        break;
    }
    return res;
}
int judge(char a)
{
    int res;
    switch (a)
    {
    case 'a':
        res = 1;
        break;
    case 'b':
        res = 2;
        break;
    case 'c':
        res = 2;
        break;
    default:
        break;
    }
    return res;
}
static void message_unpack(const unsigned char *in, unsigned int inlen, char *sendid, char *recvid, char *out)
{
    *sendid = in[1];
    *recvid = in[0];
    memcpy(out, in + 1, inlen - 1);
}
static void hello_cleanup(void)
{
    netlink_kernel_release(nl_sk);
    device_destroy(cls, devId);
    class_destroy(cls);
    unregister_chrdev_region(devId, 1);
}

static void netlink_send(int pid, uint8_t *message, int len)
{
    struct sk_buff *skb_1;
    struct nlmsghdr *nlh;

    if (!message || !nl_sk)
    {
        return;
    }

    skb_1 = alloc_skb(NLMSG_SPACE(len), GFP_KERNEL);
    if (!skb_1)
    {
        printk(KERN_ERR "alloc_skb error!\n");
    }

    nlh = nlmsg_put(skb_1, 0, 0, 0, len, 0);
    NETLINK_CB(skb_1).portid = 0;
    NETLINK_CB(skb_1).dst_group = 0;
    memcpy(NLMSG_DATA(nlh), message, len);
    netlink_unicast(nl_sk, skb_1, pid, MSG_DONTWAIT);
}

static void netlink_input(struct sk_buff *__skb)
{
    struct sk_buff *skb;
    char str[100];
    char str1[100];
    struct nlmsghdr *nlh;
    char send; //the msg sender
    char recv; //the msg receiver
    char kmsg_illegalmsg[] = "kkillegal communication!";
    char kmsg_remsg[] = "kkThe kernel has received the message!\n";

    if (!__skb)
    {
        return;
    }

    skb = skb_get(__skb);
    if (skb->len < NLMSG_SPACE(0))
    {
        return;
    }

    nlh = nlmsg_hdr(skb);
    memset(str, 0, sizeof(str));
    memcpy(str, NLMSG_DATA(nlh), sizeof(str));
    message_unpack(str, sizeof(str), &send, &recv, str1);
    printk(KERN_INFO "receive message (pid:%d):  %s\n", nlh->nlmsg_pid, str1);
    printk(KERN_INFO "the sender:%c\n", send);
    printk(KERN_INFO "receiver:%c\n", recv);
    printk(KERN_INFO "space:%d\n", NLMSG_SPACE(0));
    printk(KERN_INFO "size:%d\n", nlh->nlmsg_len);
    netlink_send(nlh->nlmsg_pid, kmsg_remsg, sizeof(kmsg_remsg));

    // if (recv == 'b')
    // {
    //     if (send == 'c')
    //         netlink_send(nlh->nlmsg_pid, kmsg_illegalmsg, sizeof(kmsg_illegalmsg));
    //     else
    //         netlink_send(101, str1, sizeof(str1));
    // }
    // else if (recv == 'c')
    // {
    //     if (send == 'b')
    //         netlink_send(nlh->nlmsg_pid, kmsg_illegalmsg, sizeof(kmsg_illegalmsg));
    //     else
    //         netlink_send(102, str1, sizeof(str1));
    // }
    // else if (recv == 'a')
    //     netlink_send(100, str1, sizeof(str1));
    // else
    // {
    //     printk(KERN_INFO "illegal communication!");
    //     netlink_send(nlh->nlmsg_pid, kmsg_illegalmsg, sizeof(kmsg_illegalmsg));
    // }
    if ((judge(recv) == 1 && judge(send) == 2) || (judge(recv) == 2 && judge(send) == 1))
    {
        /* communication between ab and ac */
        netlink_send(getpid(recv), str1, sizeof(str1));
    }
    else
    {
        printk(KERN_INFO "illegal communication!");
        netlink_send(nlh->nlmsg_pid, kmsg_illegalmsg, sizeof(kmsg_illegalmsg));
    }
}

static __init int netlink_init(void)
{
    int result;
    struct netlink_kernel_cfg nkc;
    /*create debugfs root directory*/
    // root_d = debugfs_create_dir("communiation_debugfs", NULL);
    // if (!root_d)
    // {
    //     printk(KERN_INFO "[debugfs]error create root dir\n");
    //     return 1;
    // }
    // debugfs_create_u32("normal_num", 0664, root_d, &normal_num);
    // debugfs_create_u32("illegal_num", 0664, root_d, &illegal_num);

    printk(KERN_WARNING "netlink init start!\n");
    /*注册设备节点*/
    if ((result = alloc_chrdev_region(&devId, 0, 1, "stone-alloc-dev")) != 0)
    {
        printk(KERN_WARNING "register dev id error:%d\n", result);
        goto err;
    }
    else
    {
        printk(KERN_WARNING "register dev id success!\n");
    }
    cls = class_create(THIS_MODULE, "stone-class");
    if (IS_ERR(cls))
    {
        printk(KERN_WARNING "create class error!\n");
        goto err;
    }
    if (device_create(cls, NULL, devId, "", "hello%d", 0) == NULL)
    {
        printk(KERN_WARNING "create device error!\n");
        goto err;
    }

    nkc.groups = 0;
    nkc.flags = 0;
    nkc.input = netlink_input;
    nkc.cb_mutex = NULL;
    nkc.bind = NULL;
    nkc.unbind = NULL;
    nkc.compare = NULL;
    nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST, &nkc);
    if (!nl_sk)
    {
        printk(KERN_ERR "[netlink] create netlink socket error!\n");
        goto err;
    }

    printk(KERN_ALERT "netlink init success!\n");
    return 0;
err:
    hello_cleanup();
    return -1;
}

static __exit void netlink_exit(void)
{
    hello_cleanup();
    printk(KERN_WARNING "netlink exit!\n");
}

module_init(netlink_init);
module_exit(netlink_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("tjx");
