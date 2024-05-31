#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cpufreq.h>
#include <linux/smp.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eric Kim");
MODULE_DESCRIPTION("A kernel module to get processor data.");
MODULE_VERSION("1.0");

#define PRINT_INTERVAL (60 * HZ) // 60 seconds interval

static struct timer_list my_timer;
static void my_print_cpu_info(struct timer_list *t);

static void my_print_cpu_info(struct timer_list *t) {
    unsigned int cpu;
    struct cpufreq_policy *policy;

    for_each_online_cpu(cpu) {
        policy = cpufreq_cpu_get(cpu);
        if (policy) {
            printk(KERN_INFO "CPU %u: %u MHz (min: %u MHz, max: %u MHz)\n",
                   cpu,
                   policy->cur / 1000,
                   policy->cpuinfo.min_freq / 1000,
                   policy->cpuinfo.max_freq / 1000);
            cpufreq_cpu_put(policy);
        } else {
            printk(KERN_INFO "CPU %u: No cpufreq policy\n", cpu);
        }
    }

    mod_timer(&my_timer, jiffies + PRINT_INTERVAL);
}

static int __init cpuinfo_module_init(void) {
    timer_setup(&my_timer, my_print_cpu_info, 0);
    printk(KERN_INFO "Starting CPU info timer\n");
    mod_timer(&my_timer, jiffies + PRINT_INTERVAL);
    return 0;
}

static void __exit cpuinfo_module_exit(void) {
    del_timer(&my_timer);
    printk(KERN_INFO "CPU info module exited.\n");
}

module_init(cpuinfo_module_init);
module_exit(cpuinfo_module_exit);
