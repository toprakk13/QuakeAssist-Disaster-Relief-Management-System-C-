#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}
bool QuakeAssistController::handleInitTeams(int numTeams) {
    return initializeTeams(numTeams);
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //Read the input file line by line and execute realtime.
    if (line.empty()) return true;

    std::stringstream ss(line);
    std::string command;
    ss >> command;

    if (command == "INIT_TEAMS") {
        int num;
        ss >> num;
        return handleInitTeams(num);
    } 
    else if (command == "SET_TEAM_CAPACITY") {
        int id, cap;
        ss >> id >> cap;
        return handleSetTeamCapacity(id, cap);
    }
    else if (command == "ADD_SUPPLY") {
        std::string id, city, sType;
        int amt, level;
        ss >> id >> city >> sType >> amt >> level;
        return handleAddSupply(id, city, sType, amt, level);
    }
    else if (command == "ADD_RESCUE") {
        std::string id, city, risk;
        int numP, level;
        ss >> id >> city >> numP >> risk >> level;
        return handleAddRescue(id, city, numP, risk, level);
    }
    else if (command == "REMOVE_REQUEST") {
        std::string id;
        ss >> id;
        return handleRemoveRequest(id);
    }
    else if (command == "HANDLE_EMERGENCY") {
        int tId, k;
        ss >> tId >> k;
        return handleHandleEmergency(tId, k);
    }
    else if (command == "DISPATCH_TEAM") {
        int tId;
        ss >> tId;
        return handleDispatchTeam(tId);
    }
    else if (command == "PRINT_QUEUES") {
        printQueues();
        return true;
    }
    else if (command == "PRINT_TEAM") {
        int tId;
        ss >> tId;
        printTeam(tId);
        return true;
    }
    else if (command == "CLEAR") {
        clear();
        std::cout << "System cleared." << std::endl;
        return true; 
    }
    else {
        std::cout << "Error: Unknown command " << "'"<<command <<"'"<< "." << std::endl;
        return true;
    }
    
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
    delete[] teams;
    teamCount = numTeams;
    teams = new Team[teamCount];
    
    for (int i = 0; i < teamCount; ++i) {
        teams[i].setId(i);
        
    }
    std::cout << "Initialized " << numTeams << " teams." << std::endl;
    return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.
    for (int i = 0; i < teamCount; i++) {
        if (teams[i].getId() == teamId) return i;
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //Find the index of team in the array, update the capacity value of the team.
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    teams[idx].setMaxLoadCapacity(capacity);
    std::cout << "Team " << teamId << " capacity set to " << capacity << "." << std::endl;
    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.
    Request req(id, cityStr, supplyTypeStr, amount, emergencyLevel);
    supplyQueue.enqueue(req);
    std::cout << "Request " << id << " added to SUPPLY queue." << std::endl;
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    Request req(id, cityStr, numPeople, riskStr, emergencyLevel);
    rescueQueue.enqueue(req);
    std::cout << "Request " << id << " added to RESCUE queue." << std::endl;
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //Remove request using request ID from request(SUPPLY, RESCUE) queue. 
    bool removed = supplyQueue.removeById(id);
    if (!removed) {
        removed = rescueQueue.removeById(id);
    }
    if (removed) {
        std::cout << "Request " << id << " removed from queues." << std::endl;
    } else {
        std::cout << "Error: Request " << id << " not found." << std::endl;
    }
    return true;
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.
    (void)teamId;
    (void)k;
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }

    Team& team = teams[idx];

    for (int i = 0; i < k; ++i) {
        Request supplyFront, rescueFront;
        bool hasSupply = supplyQueue.peek(supplyFront);
        bool hasRescue = rescueQueue.peek(rescueFront);

        if (!hasSupply && !hasRescue) break;

        Request* chosenReq = nullptr;
        bool pickFromSupply = false;

        if (hasSupply && !hasRescue) {
            pickFromSupply = true;
        } 
        else if (!hasSupply && hasRescue) {
            pickFromSupply = false;
        }
        else {
           
            int sScore = supplyFront.computeEmergencyScore();
            int rScore = rescueFront.computeEmergencyScore();

            if (sScore > rScore) {
                pickFromSupply = true;
            } else if (rScore > sScore) {
                pickFromSupply = false;
            } else {
                
                pickFromSupply = false;
            }
        }

        
        Request toAssign;
        if (pickFromSupply) toAssign = supplyFront;
        else toAssign = rescueFront;

        if (!team.tryAssignRequest(toAssign)) {
            
            std::cout << "Overload on Team " << teamId << ": rolling back mission." << std::endl;
            team.rollbackMission(supplyQueue, rescueQueue);
            return true; 
        } else {
            
            Request n;
            if (pickFromSupply) supplyQueue.dequeue(n);
            else rescueQueue.dequeue(n);
        }
    }
    
    
    if (team.hasActiveMission()) {
        int supplyCount = 0;
        int rescueCount = 0;
        const MissionStack& missionS = team.getMissionStack();
        Request* stackData = missionS.getData(); 
        int topIdx = missionS.getTopIndex();

        for (int i = 0; i <= topIdx; ++i) {
            if (stackData[i].getType() == "SUPPLY") {
                supplyCount++;
            } else if (stackData[i].getType() == "RESCUE") {
                rescueCount++;
            }
        }
        std::cout << "Team " << teamId << " assigned " 
                  << missionS.size() << " requests (" 
                  << supplyCount << " SUPPLY, " << rescueCount << " RESCUE), total workload " 
                  << team.getCurrentWorkload() << "." << std::endl;
    }

    
    return true;
}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //Print queues.
    std::cout << "SUPPLY QUEUE:" << std::endl;
    if (supplyQueue.isEmpty()) {
        
    } else {
        
        Request* data = supplyQueue.getData();
        int cap = supplyQueue.getCapacity();
        int frontIdx = supplyQueue.getFrontIndex();
        int sCount = supplyQueue.getCount();

        for (int i = 0; i < sCount; i++) {
            int idx = (frontIdx + i) % cap;
    
            std::cout << data[idx].getId() << " " 
                      << data[idx].getCity() << " " 
                      << data[idx].getSupplyType() << " " 
                      << data[idx].getAmount() << " " 
                      << data[idx].getEmergencyLevel() << std::endl;
        }
    }

    std::cout << "RESCUE QUEUE:" << std::endl;
    if (rescueQueue.isEmpty()) {
        
    } else {
        Request* data = rescueQueue.getData();
        int cap = rescueQueue.getCapacity();
        int frontIdx = rescueQueue.getFrontIndex();
        int rCount = rescueQueue.getCount();

        for (int i = 0; i < rCount; i++) {
            int idx = (frontIdx + i) % cap;
            
            std::cout << data[idx].getId() << " " 
                      << data[idx].getCity() << " " 
                      << data[idx].getNumPeople() << " " 
                      << data[idx].getRisk() << " " 
                      << data[idx].getEmergencyLevel() << std::endl;
        }
    }
    
}

void QuakeAssistController::printTeam(int teamId) const {
    //Print team data using teamId.
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return;
    }

    const MissionStack& ms = teams[idx].getMissionStack();
    std::cout << "TEAM " << teamId << " STACK:" << std::endl;
    
    Request* data = ms.getData();
    int topIdx = ms.getTopIndex();

    if (topIdx == -1) {
        
    } else {
        for (int i = topIdx; i >= 0; i--) {
            if(data[i].getType()=="RESCUE"){
            std::cout << data[i].getId() << " " << data[i].getCity()<<" " <<data[i].getNumPeople()<<" "<<data[i].getRisk()<<" "<<data[i].getEmergencyLevel()<< std::endl;
            }
            else if(data[i].getType()=="SUPPLY"){
                std::cout << data[i].getId() << " " << data[i].getCity()<<" " <<data[i].getSupplyType()<<" "<<data[i].getAmount()<<" "<<data[i].getEmergencyLevel()<< std::endl;
            }
        }
    }
    
    
    
}

void QuakeAssistController::clear() {
    //Clear data.
    supplyQueue.clear();
    rescueQueue.clear();
    for (int i = 0; i < teamCount; ++i) {
        teams[i].clearMission();
    }
}
