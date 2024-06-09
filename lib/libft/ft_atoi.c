/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:30:48 by rtamouss          #+#    #+#             */
/*   Updated: 2024/06/08 21:22:06 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void check_number(long long nbr, int sign, char *str)
{
	if ((nbr > 9223372036854775807 && sign == 0) || (nbr > (long long)9223372036854775808 && sign == 1))
	{
        write(2, "exit\nbash: exit: ", ft_strlen("exit\nbash: exit: "));
        write(2, str, ft_strlen(str));
        write(2, ": numeric argument required\n", ft_strlen(": numeric argument required\n"));
		free_at_exit();
		exit(2);
	}
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long long	res;

	i = 0;
	res = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - 48);
		check_number(res, sign, (char *)str);
		i++;
	}
	res *= sign;
	return (res);
}

// int	main(void)
// {
// 	int b;
// 	b = ft_atoi("     			0");
// 	printf("%d\n",b);
// 	printf("----------\n");
// 	int b1;
// 	b1 = atoi("     				0	");
// 	printf("%d\n",b1);
// 	return (0);
// }