#include <QtCore/QCoreApplication>
#include <QFileInfoList>
#include <QStringList>
#include <QDebug>
#include "trapezoidfunction.h"
#include "quantifier.h"
#include "qualitymeasures.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString dataFileName = "database.dat";
    QString dataDescFileName = "data_description.txt";

    QVector<QStringList> data;
    {
        QFile databaseFile(dataFileName);
        if (!databaseFile.open(QIODevice::ReadOnly)) {
            qCritical() << "failed to open database file" << dataFileName;
            return -2;
        }
        while (!databaseFile.atEnd() && data.size()<10000) {
            data << QString(databaseFile.readLine()).trimmed().split(QRegExp("\\s"), QString::SkipEmptyParts);
            if(data.at(data.size()-1).size()<30){
                qDebug()<<"fail "<<data.size();
            }
        }
        databaseFile.close();
    }

    QVector<QString> dataDesc;
    {
        QFile dataDescFile(dataDescFileName);
        if (!dataDescFile.open(QIODevice::ReadOnly)) {
            qCritical() << "failed to open data description file" << dataDescFileName;
            return -2;
        }
        QStringList splittedLine;
        QTextStream in(&dataDescFile);
        while (!in.atEnd()) {
            splittedLine = in.readLine().split(QString(":"));
            dataDesc << splittedLine.at(0);
        }
    }


    QList<QVector<QVariant> > dbRows = QList<QVector<QVariant> >();
    for (int i=0; i<data.size(); i++){
        if(data.at(i).size() == data.at(0).size()){
            QVector<QVariant> dbRow = QVector<QVariant>(data.at(0).size());
            for (int j=0; j<data.at(0).size(); j++){
                if(dataDesc[j]=="N"){
                    dbRow[j]=QVariant(data.at(i).at(j).toDouble());
                }else{
                    dbRow[j]=QVariant(data.at(i).at(j));
                }
            }
            dbRows.append(dbRow);
        }
    }


    /////////////////////////////////////////////
    TrapezoidFunction moreMembership = TrapezoidFunction();
    moreMembership.setA(0.5);
    moreMembership.setB(0.7);
    moreMembership.setB(1);
    moreMembership.setB(1);
    Quantifier moreQuantifier = Quantifier();
    moreQuantifier.setMembershipFunction(&moreMembership);
    moreQuantifier.setRelative(true);

    TrapezoidFunction lowRainFunction = TrapezoidFunction();
    lowRainFunction.setA(0);
    lowRainFunction.setB(0);
    lowRainFunction.setC(20);
    lowRainFunction.setD(50);
    BasicFuzzySet lowRainSet = BasicFuzzySet();
    lowRainSet.setColNum(19);
    lowRainSet.setMembershipFunction(&lowRainFunction);

    QList<BasicFuzzySet> summarizers = QList<BasicFuzzySet>();
    summarizers.append(lowRainSet);

    QList<BasicFuzzySet> qualifiers = QList<BasicFuzzySet>();

    qDebug()<<"T1: "<<QualityMeasures::computeT1(moreQuantifier, qualifiers, summarizers, dbRows);
    qDebug()<<"T2: "<<QualityMeasures::computeT2T9(summarizers, dbRows);
    qDebug()<<"T3: "<<QualityMeasures::computeT3(qualifiers, summarizers, dbRows);
    qDebug()<<"T4: "<<QualityMeasures::computeT4(qualifiers, summarizers, dbRows);
    qDebug()<<"T5: "<<QualityMeasures::computeT5T11(summarizers.size());
    qDebug()<<"T6: "<<QualityMeasures::computeT6(moreQuantifier, dbRows.size());
    qDebug()<<"T7: "<<QualityMeasures::computeT7(moreQuantifier, dbRows.size());
    qDebug()<<"T8: "<<QualityMeasures::computeT8T10(summarizers, dbRows);
    qDebug()<<"T9: "<<QualityMeasures::computeT2T9(qualifiers, dbRows);
    qDebug()<<"T10: "<<QualityMeasures::computeT8T10(qualifiers, dbRows);
    qDebug()<<"T11: "<<QualityMeasures::computeT5T11(qualifiers.size());
    return 0;
}
