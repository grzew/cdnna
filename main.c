#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>


#include "load.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grzegorz Wieczorek <grzewster@gmail.com>");
MODULE_DESCRIPTION("CDN - node alignment");
MODULE_ALIAS("cdnna");

static struct nf_hook_ops nfhookops;
struct sk_buff *sock_buff;
struct iphdr *ip_header;

unsigned int hook_function(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
  sock_buff = skb;
  ip_header = (struct iphdr *)skb_network_header(sock_buff);
  printk(KERN_INFO "Packet logged, load: %d, source: %pI4\n", getmyload(), &ip_header->saddr); //log to var/log/messages
  return NF_ACCEPT; //Accept packet
}


int init_module (void) {
	printk(KERN_INFO "CDNna loaded\n");
	nfhookops.hook = hook_function;
	nfhookops.hooknum = NF_INET_PRE_ROUTING;
	nfhookops.pf = PF_INET; //IPv4
	nfhookops.priority = NF_IP_PRI_FIRST;
	nf_register_hook(&nfhookops);
	return 0;
}

void cleanup_module(void) {
	printk(KERN_INFO "CDNna removed\n");
	nf_unregister_hook(&nfhookops);
}
