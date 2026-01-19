# QuakeAssist-Disaster-Relief-Management-System-C-
This project is a C++-based management system that simulates post-earthquake relief and rescue operations. The primary goal is to design and operate custom data structures using dynamic memory management, without relying on ready-made libraries such as STL vector, queue, or stack.

Project Features
	•	Custom Data Structures:
	•	RequestQueue: A dynamically resizable structure implementing a circular queue to manage incoming relief requests (Supply/Rescue). The queue automatically expands when capacity is reached.
	•	MissionStack: A dynamic stack structure following the LIFO (Last-In-First-Out) principle to manage tasks assigned to teams.
	•	Dynamic Memory Management:
All data structures are managed via pointers, with resize functions ensuring memory efficiency.
	•	Simulation Controller:
QuakeAssistController acts as the central manager, coordinating teams, queues, and mission assignments.

File Structure
	•	QuakeAssistController.h / .cpp: The core of the system. Processes commands (ADD_SUPPLY, DISPATCH_TEAM, etc.) and controls the logical flow.
	•	RequestQueue.h / .cpp: Queue structure for managing incoming aid requests.
	•	MissionStack.h / .cpp: Stack structure that stores teams’ task history or current missions.
	•	Team.h / .cpp: Models each rescue team’s capacity and current status.
	•	main.cpp: Entry point that reads the input file and starts the simulation.

Supported Commands

The system processes command files in the following format:
	•	INIT_TEAMS <num>: Initializes the number of teams.
	•	ADD_SUPPLY <id> <city> <type> <amount> <level>: Adds a logistics supply request.
	•	ADD_RESCUE <id> <city> <num_people> <risk> <level>: Adds a rescue request.
	•	HANDLE_EMERGENCY <team_id> <k>: Assigns k tasks from the queue to the specified team.
	•	DISPATCH_TEAM <team_id>: Completes the most recently assigned task for the team (stack pop).

Build & Run

You can compile the project using a C++ compiler such as g++ or clang:
g++ -o QuakeAssist main.cpp QuakeAssistController.cpp RequestQueue.cpp MissionStack.cpp Request.cpp Team.cpp
./QuakeAssist < example_input.txt
