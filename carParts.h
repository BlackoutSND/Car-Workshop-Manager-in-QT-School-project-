#ifndef CARPARTS_H
#define CARPARTS_H
#include <QFileInfo>

class CarParts{
    public:
    CarParts(){};
        QString getName()const{return name;};
        double getUnitPrice()const{return unitPrice;};
        double getAmount()const{return amount;};

        void setName(const QString & name){this->name=name;};
        void setAmount(const double & amount){this->amount=amount;};
        void setUnitPrice(const double & unitPrice){this->unitPrice=unitPrice;};
        inline bool operator==( const CarParts& other) {
            if(this->getName()==other.getName() && this->getAmount()==other.getAmount() && this->getUnitPrice()==other.getUnitPrice()) {
                return true;
            }
            return false;
        }
    private:
        QString name;
        double amount;
        double unitPrice;
};

#endif // CARPARTS_H
