#ifndef TICKET_H
#define TICKET_H

#include <vector>
#include <QTSql>
#include <QDebug>
#include <QFileInfo>
#include "carParts.h"
#include "car.h"

class Ticket{
    public:
        Ticket(){};
        int getId()const{return id;};
        QString getProblemDetails()const{return problemDetails;};
        std::vector<int> getassignedEmpIds()const{return assignedEmpIds;};
        std::vector<std::pair<QDate,std::vector<int>>> getSchedRepairTime()const{return schedRepairTime;};


        std::vector<CarParts> getParts()const{return Parts;};
        Car getCarDetailes()const{return carDetailes;};
        QString getDeducedProblem()const{return deducedProblem;};
        double getExpectedCost()const{return expectedCost;};
        double getTotalPrice()const{return totalPrice;};
        double getPaidByClient()const{return paidByClient;};
        bool getCostIsAccepted()const{return costIsAccepted;};
        QString getStatus()const{return status;};


        void setId(int newId) {
            id = newId;
        }

        void setProblemDetails(const QString& newProblemDetails) {
            problemDetails = newProblemDetails;
        }

        void setassignedEmpIds(const std::vector<int>& newassignedEmpIds) {
            assignedEmpIds = newassignedEmpIds;
        }

        void setSchedRepairTime(const std::vector<std::pair<QDate,std::vector<int>>>& newSchedRepairTime) {
            schedRepairTime = newSchedRepairTime;
        }


        void setParts(const std::vector<CarParts>& newParts) {
            Parts = newParts;
        }
        void setCarDetailes(const Car& newCarDetailes) {
            carDetailes = newCarDetailes;
        }
        void setDeducedProblem(const QString& deducedProblem) {
            this->deducedProblem = deducedProblem;
        }
        void setExpectedCost(double newExpectedCost) {
            expectedCost = newExpectedCost;
        }

        void setTotalPrice(double newTotalPrice) {
            totalPrice = newTotalPrice;
        }

        void setPaidByClient(double newPaidByClient) {
            paidByClient = newPaidByClient;
        }

        void setCostIsAccepted(bool newCostIsAccepted) {
            costIsAccepted = newCostIsAccepted;
        }
        void setStatus(const QString& status) {
            this->status = status;
        }

        void addSchedRepairTime(const std::pair<QDate,std::vector<int>>& dateSlotsPair)
        {
            schedRepairTime.push_back(dateSlotsPair);
        }
        void recalExpectedCost(){
            expectedCost = 0;
            for (size_t i = 0; i < this->Parts.size(); ++i) {
                this->expectedCost += this->Parts[i].getAmount()*this->Parts[i].getUnitPrice();
            }

        }

        void addAssignedEmpIds(const int assignedEmpId){this->assignedEmpIds.push_back(assignedEmpId);}
        void remAssignedEmpId(const int assignedEmpId){
            //this->assignedEmpIds.erase(std::find(this->assignedEmpIds.begin(),this->assignedEmpIds.end(),assignedEmpId));
            auto it = std::find(this->assignedEmpIds.begin(),this->assignedEmpIds.end(),assignedEmpId);
            if(it == this->assignedEmpIds.end())
                return;
            else
                this->assignedEmpIds.erase(it);
        }
        void remCarPart(const CarParts carPart){
            //this->assignedEmpIds.erase(std::find(this->assignedEmpIds.begin(),this->assignedEmpIds.end(),assignedEmpId));
            auto it = std::find(this->Parts.begin(),this->Parts.end(),carPart);
            if(it == this->Parts.end())
                return;
            else
                this->Parts.erase(it);
        }
        void addCarPart(const CarParts& part){this->Parts.push_back(part);}
    private:
        int id;
        QString problemDetails;
        std::vector<int> assignedEmpIds;
        std::vector<std::pair<QDate,std::vector<int>>> schedRepairTime;
        std::vector<CarParts> Parts;
        Car carDetailes;
        QString deducedProblem;
        double expectedCost;
        double totalPrice;
        double paidByClient;
        bool costIsAccepted;
        QString status;

};

#endif // TICKET_H
