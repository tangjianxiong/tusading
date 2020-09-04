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

#define NETLINK_TEST (25)

static dev_t devId;
static struct class *cls = NULL;
struct sock *nl_sk = NULL;

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
    //allocate memory for skb_1
    skb_1 = alloc_skb(NLMSG_SPACE(len), GFP_KERNEL);
    if (!skb_1)
    {
        printk(KERN_ERR "alloc_skb error!\n");
    }
    //set the message header
    nlh = nlmsg_put(skb_1, 0, 0, 0, len, 0);
    NETLINK_CB(skb_1).portid = 0;
    NETLINK_CB(skb_1).dst_group = 0;
    //fill message
    memcpy(NLMSG_DATA(nlh), message, len);
    //send messages via unicast
    netlink_unicast(nl_sk, skb_1, pid, MSG_DONTWAIT);
}

static void netlink_input(struct sk_buff *__skb)
{
    struct sk_buff *skb;
    char str[1024];
    struct nlmsghdr *nlh;

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
    printk(KERN_INFO "receive message (pid:%d):%s\n", nlh->nlmsg_pid, str);
    printk(KERN_INFO "space:%d\n", NLMSG_SPACE(0));
    printk(KERN_INFO "size:%d\n", nlh->nlmsg_len);
    if (nlh->nlmsg_pid == 100)
    {
        netlink_send(101, NLMSG_DATA(nlh), nlh->nlmsg_len - NLMSG_SPACE(0));
    }
    else
    {
        netlink_send(100, NLMSG_DATA(nlh), nlh->nlmsg_len - NLMSG_SPACE(0));
    }

    return;
}

static __init int netlink_init(void)
{
    int result;
    struct netlink_kernel_cfg nkc;

    printk(KERN_WARNING "netlink init start!\n");

    //��̬ע���豸��
    if ((result = alloc_chrdev_region(&devId, 0, 1, "stone-alloc-dev")) != 0)
    {
        printk(KERN_WARNING "register dev id error:%d\n", result);
        goto err;
    }
    else
    {
        printk(KERN_WARNING "register dev id success!\n");
    }
    //��̬�����豸�ڵ�
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

    //���ýṹ
    nkc.groups = 0; //����
    nkc.flags = 0;
    nkc.input = netlink_input; //�󶨽��պ���
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
MODULE_AUTHOR("Stone");
