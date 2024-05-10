#ifndef CAR_H
#define CAR_H

#include <QFileInfo>

class Car{
public:
    Car(){};
    QString getBrand()const{return brand;};
    QString getModel()const{return model;};
    QString getRegId()const{return regId;};

    void setBrand(const QString & brand){this->brand=brand;};
    void setModel(const QString & model){this->model=model;};
    void setRegId(const QString & regId){this->regId=regId;};

private:
    QString brand;
    QString model;
    QString regId;
};


#endif // CAR_H
