************** READ ME **************

Author:
        + Patrick Chau - pchau10@csu.fullerton.edu	
        + Le Do – doleminhtriet@csu.fullerton.edu
        + Andrew Ho - andrewhostudent@csu.fullerton.edu
        + Alexander Peras - alopp5570@csu.fullerton.edu
        

+ Github: https://github.com/doleminhtriet/cpsc335_project2




****** Prerequisites ******
            
Prerequisites: C++ Compiler

****** Steps to Run the Code: ******

1. Place input.txt (which contains test cases) and Project2_starter.cpp (the code file) in the same directory.

2. Compile the code by running: g++ Project2_starter.cpp -o Project2

3. Run the compiled executable and check the output: ./Project2

4. The program will read from input.txt and write the results to output.txt.

5. To verify the output, open output.txt in the same directory to see the available time slots found by the program.

****** Algorithm ****** 

***** Input *****
person1_Schedule = [['09:00', '10:30'], ['12:00', '13:30'], ['16:00', '17:00']]
person1_DailyAct = ['09:00', '18:00']
person2_Schedule = [['08:30', '09:30'], ['13:00', '14:00'], ['15:30', '16:30']]
person2_DailyAct = ['09:00', '17:30']
duration_of_meeting = 30


***** Output *****
Available Slots:
10:30 - 12:00
14:00 - 15:30
17:00 - 17:30


***** Pseudocode *****

def findAvailableSlots(schedules, workingPeriods, duration):
    # Initialize an empty list to store available slots
    availableSlots = []
    # Step 1: Determine the overall working hours based on the latest login and earliest logout
    overallStart = latest start time among all working periods
    overallEnd = earliest end time among all working periods
    # If overall working hours are less than the meeting duration, return an empty list
    if overallEnd - overallStart < duration:
        return availableSlots
   # Step 2: Gather all busy intervals from individual schedules
    busyTimes = []
    for schedule in schedules:
        for interval in schedule:
            busyTimes.append(interval)  # Add each busy interval to busyTimes
    # Step 3: Sort all busy intervals by their start time
    busyTimes.sort()
    # Step 4: Find gaps between busy intervals that fit within the working hours and required duration
    startTime = overallStart  # Start checking from the beginning of the working hours
    for interval in busyTimes:
        # Check if there's a gap between the current interval and the start time
        if interval.start > startTime and interval.start <= overallEnd:
            endTime = min(interval.start, overallEnd)
            # If the gap is large enough for the required duration, add it as an available slot
            if endTime - startTime >= duration:
                availableSlots.append((startTime, endTime))
        # Move the start time forward to the end of the current interval
        startTime = max(startTime, interval.end)
        # Stop checking if we've reached the end of the working hours
        if startTime >= overallEnd:
            break
    # Step 5: If there's a gap at the end of the busy intervals, add it as the final available slot
    if overallEnd - startTime >= duration and startTime < overallEnd:
        availableSlots.append((startTime, overallEnd))
    return availableSlots



 ******************************************* END ***********************************************