# cpsc335_project2
cpsc335_project2

**Project 2: Implementing Algorithm**
Fall 2024 CPSC 335 - Algorithm Engineering

**Abstract**
Develop a pseudocode for an algorithm; analyze your pseudocode mathematically; implement the
code for the algorithm of your choice; test your implementation; and describe your results.
The Problem: Matching Group Schedules
The group schedule matching takes two or more arrays as input. The arrays represent slots that are
already booked and the login/logout time of group members. It outputs an array containing intervals
of time when all members are available for a meeting for a minimum duration expected.

**The group schedule matching takes the following inputs:**
1. Busy_Schedule: An array list that represents the person's existing schedule (they can’t plan
any other engagement during these hours)
Hint: Array may be 2D or maybe a list, ArrayList.
2. Working_period: Daily working periods of group members. (login, logout)
Just two entries [login, logout]
3. Duration of the meeting It outputs a list of list containing intervals of time when all
members are available for a meeting for the minimum duration of the meeting required.

**An analogy for the question:**
Assume you and your group members provide your schedules and daily availability. The goal is
to find a time slot when all of you are free for a meeting, considering the provided schedules and
the minimum duration required for the meeting.

**Sample Input**
Enter person1_Schedule =[[ ‘7:00’, ’8:30’], [’12:00’, ’13:00’], [’16:00’, ’18:00’]]
person1_DailyAct = [‘9:00’, ’19:00’]
Enter person2_Schedule = [[ ‘9:00’, ’10:30’], [’12:20’, ’13:30’], [’14:00’, ’15:00’], [’16:00’, ’17:00’ ]]
person2_DailyAct = [‘9:00’, ’18: 30’]
Enter duration_of_meeting =30


**Sample output**
[[’10:30’, ’12:00’], [’15:00’, ’16:00’], [’18:00’, ’18:30’]]

