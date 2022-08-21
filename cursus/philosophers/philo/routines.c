#include "philo.h"

static void	philo_sleep(t_philo *philo)
{
	long	time;

	if (is_dead(philo))
		return ;
	time = current_time();
	mtx_print(passed(time, philo->table->tv_start),
			philo, "is sleeping");
	if (philo->status != 4)
		philo->tv_last_act = time;
	philo->status = 3;
	if (philo->table->options[3] < philo->table->options[1])
		usleep(philo->table->options[3] * 1000);
	else
		usleep((philo->table->options[1]
				- passed(current_time(), philo->tv_last_act)) * 1000);
}

static void	drop_forks(t_philo *philo)
{
	if (is_dead(philo))
			return ;
	philo->tv_last_act = current_time();
	philo->status = 4;
	while (philo->forks > 0)
	{
		if (is_dead(philo))
			return ;
		pthread_mutex_lock(&philo->table->ffork_mtx);
		philo->forks -= 2;
		philo->table->free_forks += 2;
		mtx_print(passed(current_time(), philo->table->tv_start),
				philo, "has dropped two forks");
		pthread_mutex_unlock(&philo->table->ffork_mtx);
	}
	philo_sleep(philo);
}

static void	eat(t_philo *philo)
{
	long	time;

	while (philo->forks < 2)
	{
		if (is_dead(philo))
			return ;
		pthread_mutex_lock(&philo->table->ffork_mtx);
		if (philo->table->free_forks > 0)
		{
			philo->table->free_forks -= 2;
			philo->forks += 2;
			time = current_time();
			mtx_print(passed(time, philo->table->tv_start),
					philo, "has taken two forks");
		}
		pthread_mutex_unlock(&philo->table->ffork_mtx);
	}
	mtx_print(passed(time, philo->table->tv_start),
			philo, "is eating");
	philo->tv_last_act = time;
	philo_eating_status(philo);
	usleep(philo->table->options[2] * 1000);
	drop_forks(philo);
}

void	*meal(void	*param)
{
	t_philo	*philo;
	long	time;

	philo = (t_philo *) param;
	philo->tv_last_act = current_time();
	if (philo->index % 2 == 0)
		eat(philo);
	else
		philo_sleep(philo);
	while (!is_dead(philo) && !is_finished(philo))
	{
		time = current_time();
		if (time - philo->table->tv_start > 10 ||
				(time - philo->table->tv_start) / 2 > philo->table->options[1])
			eat(philo);
		else if (philo->status == 3 &&
				time - philo->table->tv_start > philo->table->options[3])
		{
			mtx_print(passed(time, philo->table->tv_start),
					philo, "is thinking");
			philo->status = 4;
		}
	}	
	return (NULL);
}
