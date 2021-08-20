/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getDifferenceDates.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 09:33:25 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/20 09:36:43 by maraurel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

const int monthDays[12]
    = { 31, 28, 31, 30, 31, 30,
       31, 31, 30, 31, 30, 31 };

int countLeapYears(struct Date2 d)
{
    int years = d.y;
    if (d.m <= 2)
        years--;
    return years / 4
           - years / 100
           + years / 400;
}

int getDifferenceDates(struct Date2 dt1, struct Date2 dt2)
{
    long int n1 = dt1.y * 365 + dt1.d;
    for (int i = 0; i < dt1.m - 1; i++)
        n1 += monthDays[i];
    n1 += countLeapYears(dt1);
    long int n2 = dt2.y * 365 + dt2.d;
    for (int i = 0; i < dt2.m - 1; i++)
        n2 += monthDays[i];
    n2 += countLeapYears(dt2);
    return (n2 - n1);
}
