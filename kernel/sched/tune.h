
#ifdef CONFIG_SCHED_TUNE

#include <linux/reciprocal_div.h>

/*
 * System energy normalization constants
 */
struct target_nrg {
	unsigned long min_power;
	unsigned long max_power;
	struct reciprocal_value rdiv;
};

int schedtune_cpu_boost(int cpu);
int schedtune_task_boost(struct task_struct *tsk);
int schedtune_task_boost_rcu_locked(struct task_struct *tsk);

int schedtune_prefer_idle(struct task_struct *tsk);

void schedtune_enqueue_task(struct task_struct *p, int cpu);
void schedtune_dequeue_task(struct task_struct *p, int cpu);

unsigned long boosted_cpu_util(int cpu, unsigned long other_util);

#else /* CONFIG_SCHED_TUNE */

#define schedtune_cpu_boost(cpu)  0
#ifndef CONFIG_UCLAMP_TASK
#define schedtune_task_boost(tsk) 0
#define schedtune_task_boost_rcu_locked(tsk) 0
#else
#define schedtune_task_boost(tsk) uclamp_boosted(tsk)
#define schedtune_task_boost_rcu_locked(tsk) uclamp_boosted(tsk)
#endif

#ifndef CONFIG_UCLAMP_TASK_GROUP
#define schedtune_prefer_idle(tsk) 0
#else
#define schedtune_prefer_idle(tsk) uclamp_latency_sensitive(tsk)
#endif

#define schedtune_enqueue_task(task, cpu) do { } while (0)
#define schedtune_dequeue_task(task, cpu) do { } while (0)

#define boosted_cpu_util(cpu, other_util) (cpu_util_cfs(cpu_rq(cpu)) + other_util)

#endif /* CONFIG_SCHED_TUNE */
