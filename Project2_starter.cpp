#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>

using namespace std;

// Helper function to convert minutes to HH:MM format
string minutesToTime(int totalMinutes)
{
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    ostringstream oss;
    oss << setw(2) << setfill('0') << hours << ":"
        << setw(2) << setfill('0') << minutes;
    return oss.str();
}

// Function to convert HH:MM format to total minutes, with error handling
int timeToMinutes(const string &time)
{
    string cleanTime = time;
    cleanTime.erase(remove(cleanTime.begin(), cleanTime.end(), '\''), cleanTime.end()); // Remove quotes
    cleanTime.erase(remove(cleanTime.begin(), cleanTime.end(), '"'), cleanTime.end());  // Remove quotes
    try
    {
        int hours = stoi(cleanTime.substr(0, 2));
        int minutes = stoi(cleanTime.substr(3, 2));
        return hours * 60 + minutes;
    }
    catch (const invalid_argument &)
    {
        cerr << "Error: Invalid time format encountered: " << cleanTime << endl;
        exit(1);
    }
    catch (const out_of_range &)
    {
        cerr << "Error: Time value out of range: " << cleanTime << endl;
        exit(1);
    }
}

// Function to parse schedule strings into a vector of time intervals in minutes
vector<pair<int, int>> parseSchedule(const string &scheduleStr)
{
    vector<pair<int, int>> intervals;
    size_t startPos = 0, endPos = 0;

    while ((startPos = scheduleStr.find('\'', startPos)) != string::npos)
    {
        endPos = scheduleStr.find('\'', startPos + 1);
        string startStr = scheduleStr.substr(startPos + 1, endPos - startPos - 1);
        startPos = scheduleStr.find('\'', endPos + 1) + 1;
        endPos = scheduleStr.find('\'', startPos);
        string endStr = scheduleStr.substr(startPos, endPos - startPos);

        int start = timeToMinutes(startStr);
        int end = timeToMinutes(endStr);
        intervals.emplace_back(start, end);
        startPos = endPos + 1;
    }

    return intervals;
}

// Function to find available slots based on schedules, common working period, and duration
vector<pair<int, int>> findAvailableSlots(
    const vector<vector<pair<int, int>>> &schedules, // Collection of schedules for all individuals, each with a vector of busy intervals
    const pair<int, int> &commonWorkingPeriod,       // Common working hours interval (start and end time)
    int duration                                     // Minimum duration required for an available slot
)
{
    vector<pair<int, int>> availableSlots;        // Vector to store resulting available slots for the meeting
    int overallStart = commonWorkingPeriod.first; // Start of the common working period
    int overallEnd = commonWorkingPeriod.second;  // End of the common working period

    // Step 1: Gather all busy intervals from each person's schedule
    vector<pair<int, int>> busyTimes;
    for (const auto &schedule : schedules)
    { // Iterate over each individual's schedule
        for (const auto &interval : schedule)
        {                                  // Iterate over each busy interval in the schedule
            busyTimes.push_back(interval); // Collect all intervals into the busyTimes vector
        }
    }

    // Step 2: Sort all busy intervals by their start times
    sort(busyTimes.begin(), busyTimes.end()); // Sorting is necessary for efficient processing of intervals to find gaps

    int startTime = overallStart; // Initialize startTime to the beginning of the common working period

    // Step 3: Iterate over sorted busy intervals to find gaps (free slots)
    for (const auto &interval : busyTimes)
    {
        // Check if there's a gap before the current busy interval
        if (interval.first > startTime)
        {                                                  // If the current busy interval starts after startTime, there's a gap
            int endTime = min(interval.first, overallEnd); // End time for this gap is either the start of busy interval or overallEnd
            if (endTime - startTime >= duration)
            {                                                    // Check if the gap is at least as long as the required duration
                availableSlots.emplace_back(startTime, endTime); // If yes, add this interval to available slots
            }
        }
        startTime = max(startTime, interval.second); // Move startTime forward to the end of the current busy interval
        if (startTime >= overallEnd)
            break; // Stop if startTime goes beyond the common working period
    }

    // Step 4: Check for any remaining available slot between the last busy interval and the end of the working period
    if (overallEnd - startTime >= duration && startTime < overallEnd)
    {
        availableSlots.emplace_back(startTime, overallEnd); // Add any final gap as an available slot if it meets the duration requirement
    }

    return availableSlots; // Return all identified available slots
}

int main()
{
    ifstream inputFile("input.txt");
    if (!inputFile)
    {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    string line;
    vector<vector<pair<int, int>>> schedules;
    pair<int, int> commonWorkingPeriod = {0, 24 * 60}; // Default to full day
    int duration = 0;

    // Read person schedules and daily activity hours from input file
    while (getline(inputFile, line))
    {
        line.erase(remove(line.begin(), line.end(), ' '), line.end()); // Remove extra spaces
        if (line.find("Schedule") != string::npos)
        {
            size_t pos = line.find('=') + 1;
            vector<pair<int, int>> schedule = parseSchedule(line.substr(pos));
            schedules.push_back(schedule);
        }
        else if (line.find("DailyAct") != string::npos)
        {
            size_t pos = line.find('[') + 1;
            size_t commaPos = line.find(',', pos);
            string startStr = line.substr(pos, commaPos - pos);
            string endStr = line.substr(commaPos + 1, line.find(']', commaPos) - commaPos - 1);
            int start = timeToMinutes(startStr);
            int end = timeToMinutes(endStr);
            commonWorkingPeriod.first = max(commonWorkingPeriod.first, start);
            commonWorkingPeriod.second = min(commonWorkingPeriod.second, end);
        }
        else if (line.find("duration_of_meeting") != string::npos)
        {
            size_t pos = line.find('=') + 1;
            try
            {
                duration = stoi(line.substr(pos));
            }
            catch (const invalid_argument &)
            {
                cerr << "Error: Invalid duration format in input." << endl;
                exit(1);
            }
        }
    }

    inputFile.close();

    // Find available slots based on parsed data
    vector<pair<int, int>> availableSlots = findAvailableSlots(schedules, commonWorkingPeriod, duration);

    // Output available slots to console
    cout << "Available Slots:\n";
    if (availableSlots.empty())
    {
        cout << "No available slots found.\n";
    }
    else
    {
        for (const auto &slot : availableSlots)
        {
            cout << minutesToTime(slot.first) << " - " << minutesToTime(slot.second) << "\n";
        }
    }

    return 0;
}
