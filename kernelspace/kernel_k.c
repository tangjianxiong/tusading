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
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/err.h>
#include <linux/sched.h>
#define NETLINK_TEST (25)
#define PID_A (100)
#define PID_B (101)
#define PID_C (102)
#define DATA_MSG 'm'
#define DATA_CON 'r'
#define DATA_KMSG 'k'
#define DATA_HASH 'h'
#define DATA_FILE 'f'
#define NAME_A 'a'
#define NAME_B 'b'
#define NAME_C 'c'
//thread
static int get_state_i = 1;
static struct task_struct *test_task;

static dev_t devId;
static struct class *cls = NULL;
struct sock *nl_sk = NULL;
static u32 normal_num;
static u32 illegal_num;
static u32 connect_sign_b = 0;
static u32 connect_sign_c = 0;
static struct dentry *root_d;
/*通过内核线程检测应用程序是否在线*/

static int getconsign(char a)
{
    int res;
    switch (a)
    {
    case NAME_B:
        res = connect_sign_b;
        break;
    case NAME_C:
        res = connect_sign_c;
        break;
    default:
        res = -1;
        break;
    }
    return res;
}
static int getpid(char a)
{
    int res;
    switch (a)
    {

    case NAME_A:
        res = PID_A;
        break;
    case NAME_B:
        res = PID_B;
        break;

    case NAME_C:
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
static void message_unpack(const unsigned char *in, unsigned int inlen, char *sendid, char *recvid, char *msgtype, char *out)
{
    *sendid = in[1];
    *recvid = in[0];
    *msgtype = in[2];
    memcpy(out, in + 1, inlen - 1);
}
static void hello_cleanup(void)
{
    netlink_kernel_release(nl_sk);
    device_destroy(cls, devId);
    class_destroy(cls);
    unregister_chrdev_region(devId, 1);
    get_state_i = 0;
    kthread_stop(test_task);
    test_task = NULL;
    debugfs_remove_recursive(root_d);
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
    char msgtype;
    char kmsg_illegalmsg[] = "kkillegal communication!";
    char kmsg_remsg[] = "kkThe kernel has received the message!";
    char kmsg_conerr[] = "kkConnection has not been established!";

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
    message_unpack(str, sizeof(str), &send, &recv, &msgtype, str1);
    printk(KERN_INFO "[receive message (pid:%d)]:%s\n", nlh->nlmsg_pid, str1);
    printk(KERN_INFO "[the sender]:%c\n", send);
    printk(KERN_INFO "[receiver]:%c\n", recv);
    printk(KERN_INFO "[type]:%c\n", msgtype);
    printk(KERN_INFO "[space]:%d\n", NLMSG_SPACE(0));
    printk(KERN_INFO "[size]:%d\n", nlh->nlmsg_len);
    netlink_send(nlh->nlmsg_pid, kmsg_remsg, sizeof(kmsg_remsg));
    // switch (msgtype)
    // {
    // case DATA_CON:
    //     if (send == NAME_A)
    //     {
    //         if ((str1[0] == 'y') && (recv == NAME_B))
    //             connect_sign_b = 1;
    //         if ((str1[0] == 'y') && (recv == NAME_C))
    //             connect_sign_c = 1;
    //     }
<<<<<<< HEAD
    //     netlink_send(getpid(recv), str1, sizeof(str1));

=======

    //         netlink_send(getpid(recv), str1, sizeof(str1));
>>>>>>> 64e330e7f156abcf8965e85fbc10b301a04ca3de
    //     break;
    // default:
    if ((judge(recv) == 1 && judge(send) == 2) || (judge(recv) == 2 && judge(send) == 1))
    {
        // if (recv == NAME_A)
        // {
        //     if (getconsign(send) == 1)
        //         netlink_send(getpid(recv), str1, sizeof(str1));
        //     else
        //     {
        //         netlink_send(nlh->nlmsg_pid, kmsg_conerr, sizeof(kmsg_conerr));
        //     }
        // }
        // else
        // {
        //     if (getconsign(recv) == 1)
        //         netlink_send(getpid(recv), str1, sizeof(str1));
        //     else
        //     {
        //         netlink_send(nlh->nlmsg_pid, kmsg_conerr, sizeof(kmsg_conerr));
        //     }
        // }
        netlink_send(getpid(recv), str1, sizeof(str1));
    }
    else
    {
        printk(KERN_INFO "illegal communication!");
        netlink_send(nlh->nlmsg_pid, kmsg_illegalmsg, sizeof(kmsg_illegalmsg));
    }
<<<<<<< HEAD
    //     break;
    // }
=======
    //         break;
    //     }
>>>>>>> 64e330e7f156abcf8965e85fbc10b301a04ca3de
}
static int thread_get_pstate(void *data)
{
    int state_a = 0;
    int state_b = 0;
    int state_c = 0;
    struct task_struct *p;
    printk(KERN_INFO "Hello,world!\n");
    while (get_state_i)
    {
        state_a = 0;
        state_b = 0;
        state_c = 0;
        for_each_process(p)
        {
            if (strcmp(p->comm, "server_a") == 0)
                state_a = p->state;
            if (strcmp(p->comm, "client_b") == 0)
                state_b = p->state;
            if (strcmp(p->comm, "client_c") == 0)
                state_c = p->state;
        }
        printk(KERN_INFO "\n---------state---------\n");
        printk(KERN_INFO "the A is in state:[%d]\n", state_a);
        printk(KERN_INFO "the B is in state:[%d]\n", state_b);
        printk(KERN_INFO "the C is in state:[%d]\n", state_c);
        printk(KERN_INFO "---------state---------\n\n");
        ssleep(10);
    }

    return 0;
}
static __init int netlink_init(void)
{
    int result;
    int err;
    struct netlink_kernel_cfg nkc;
    /*create debugfs root directory*/
    root_d = debugfs_create_dir("communiation_debugfs", NULL);
    if (!root_d)
    {
        printk(KERN_INFO "[debugfs]error create root dir\n");
        return 1;
    }
    debugfs_create_u32("normal_num", 0664, root_d, &normal_num);
    debugfs_create_u32("illegal_num", 0664, root_d, &illegal_num);
    debugfs_create_u32("connect_sign_b", 0664, root_d, &connect_sign_b);
    debugfs_create_u32("connect_sign_c", 0664, root_d, &connect_sign_c);

    printk(KERN_WARNING "netlink init start!\n");
    /*注册设备节点*/
    if ((result = alloc_chrdev_region(&devId, 0, 1, "tjx-alloc-dev")) != 0)
    {
        printk(KERN_WARNING "register dev id error:%d\n", result);
        goto err;
    }
    else
    {
        printk(KERN_WARNING "register dev id success!\n");
    }
    cls = class_create(THIS_MODULE, "tjx-class");
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
        printk(KERN_ERR "[netlink]create netlink socket error!\n");
        goto err;
    }

    printk(KERN_ALERT "[netlink]netlink init success!\n");
    /*create thread to monitor A，B and C*/
    test_task = kthread_create(thread_get_pstate, NULL, "test_task");

    if (IS_ERR(test_task))
    {
        printk("Unable to start kernel thread.\n");
        err = PTR_ERR(test_task);
        test_task = NULL;
        return err;
    }
    wake_up_process(test_task);
    return 0;
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
