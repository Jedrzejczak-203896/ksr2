#include "qualitymeasures.h"
#include "intersection.h"
#include "support.h"

#include <QList>
#include <QVector>
#include <QVariant>
#include <cmath>

double QualityMeasures::computeT1(const Quantifier &quantifier, const QList<const BasicFuzzySet *> &qualifiers, const QList<const BasicFuzzySet *> &summarizers, const QList<QVector<QVariant> > &dbRows) {
	//int dbSize = summarizers.at(0).getElements().size();
	const FuzzySet *summarizerIntersection = summarizers.at(0);
	const FuzzySet *qualifierIntersection;
    Intersection *temp;
    for(int i = 1; i<summarizers.size(); i++){
        temp = new Intersection();
        temp->setFuzzySet1(summarizerIntersection);
		temp->setFuzzySet2(summarizers.at(i));
        summarizerIntersection = temp;
    }
    if(!qualifiers.empty()){
		qualifierIntersection = qualifiers.at(0);
        for(int i = 1; i<qualifiers.size(); i++){
            temp = new Intersection();
            temp->setFuzzySet1(qualifierIntersection);
			temp->setFuzzySet2(qualifiers.at(i));
            summarizerIntersection = temp;
        }
    }
    int m=1;
    if (quantifier.isRealtive()){
        m=dbRows.size();
    }

    if(qualifiers.empty()){
        double sum = 0;
        for(int i=0; i<dbRows.size(); i++){
            sum += summarizerIntersection->membership(dbRows.at(i));
        }
        return quantifier.membership(sum/m);
    }else{
		Intersection *intersection = new Intersection();
		intersection->setFuzzySet1(summarizerIntersection);
		intersection->setFuzzySet2(qualifierIntersection);

        double sum1=0;
        double sum2=0;

        for(int i=0; i<dbRows.size(); i++){
			sum1 += intersection->membership(dbRows.at(i));
			sum2 += qualifierIntersection->membership(dbRows.at(i));
        }
        if(sum2 == 0){
            return quantifier.membership(0);
        } else{
            return quantifier.membership((sum1/sum2)/m);
        }
    }
}

double QualityMeasures::computeT2T9(const QList<const BasicFuzzySet *> &fuzzySets, const QList<QVector<QVariant> > &dbRows){
    if(fuzzySets.empty()){
        return 1;
    }

    double result = 1;
	Support support;
    for(int i =0; i<fuzzySets.size(); i++){
        support.setFuzzySet(fuzzySets[i]);
		result*=support.cardinality(dbRows)/dbRows.size();
    }
    result = pow(result, 1.0/fuzzySets.size());
    return 1.0 - result;
}

double QualityMeasures::computeT3(const QList<const BasicFuzzySet *> &qualifiers, const QList<const BasicFuzzySet *> &summarizers, const QList<QVector<QVariant> > &dbRows){
	const FuzzySet *summarizerIntersection = summarizers.at(0);
	const FuzzySet *qualifierIntersection;
    Intersection *temp;
    for(int i = 1; i<summarizers.size(); i++){
        temp = new Intersection();
		temp->setFuzzySet1(summarizerIntersection);
		temp->setFuzzySet2(summarizers.at(i));
        summarizerIntersection = temp;
    }
    if(!qualifiers.empty()){
		qualifierIntersection = qualifiers.at(0);
        for(int i = 1; i<qualifiers.size(); i++){
            temp = new Intersection();
            temp->setFuzzySet1(qualifierIntersection);
			temp->setFuzzySet2(qualifiers.at(i));
            summarizerIntersection = temp;
        }
    }

    if(qualifiers.empty()){
        Support support = Support();
        support.setFuzzySet(summarizerIntersection);
        return support.cardinality(dbRows)/dbRows.size();
    } else{
        Intersection intersection = Intersection();
        intersection.setFuzzySet1(summarizerIntersection);
        intersection.setFuzzySet2(qualifierIntersection);

        Support support1 = Support();
        Support support2 = Support();
        support1.setFuzzySet(summarizerIntersection);
        support2.setFuzzySet(qualifierIntersection);

        return support1.cardinality(dbRows)/support2.cardinality(dbRows);
    }
}

double QualityMeasures::computeT4(const QList<const BasicFuzzySet *> &qualifiers, const QList<const BasicFuzzySet *> &summarizers, const QList<QVector<QVariant> > &dbRows){
    if(summarizers.empty()){
		return 1;
    }

    double result = 1;
    for(int i =0; i<summarizers.size(); i++){
        Support support = Support();
        support.setFuzzySet(summarizers[i]);
        result*=support.cardinality(dbRows)/dbRows.size();
    }
    result = result/summarizers.size();
    return result - QualityMeasures::computeT3(qualifiers, summarizers, dbRows);
}

double QualityMeasures::computeT5T11(const int &summarizersNum){
    if(summarizersNum==0){
        return 1;
    }
    return 2.0*pow(0.5, summarizersNum);
}

double QualityMeasures::computeT6(const Quantifier &quantifier, const int &dbRowsSize){
	QList<QVariant> quantityList;
	quantityList.reserve(dbRowsSize);
    if(quantifier.isRealtive()){
        for(int i =0; i<dbRowsSize; i++){
			QVariant qVariant = QVariant((double)i / qreal(dbRowsSize));
            quantityList.append(qVariant);
        }
    } else{
        for(int i =0; i<dbRowsSize; i++){
            QVariant qVariant = QVariant((double)i);
            quantityList.append(qVariant);
        }
    }

    Support support = Support();
    support.setFuzzySet(&quantifier);

    double result = support.cardinality(quantityList)/dbRowsSize;

    return 1 - result;
}

double QualityMeasures::computeT7(const Quantifier &quantifier, const int &dbRowsSize){
	QList<QVariant> quantityList;
	quantityList.reserve(dbRowsSize);
    if(quantifier.isRealtive()){
        for(int i =0; i<dbRowsSize; i++){
			QVariant qVariant = QVariant((double)i / qreal(dbRowsSize));
            quantityList.append(qVariant);
        }
    } else{
        for(int i =0; i<dbRowsSize; i++){
            QVariant qVariant = QVariant((double)i);
            quantityList.append(qVariant);
        }
    }

    return quantifier.cardinality(quantityList)/quantityList.size();
}

double  QualityMeasures::computeT8T10(QList<const BasicFuzzySet *> fuzzySets, const QList<QVector<QVariant> > &dbRows){
    if(fuzzySets.empty()){
        return 1;
    }

    double result = 1;
	Support support;
    for(int i =0; i<fuzzySets.size(); i++){
        support.setFuzzySet(fuzzySets[i]);
        result*=support.cardinality(dbRows)/dbRows.size();
    }
    result = pow(result, 1.0/fuzzySets.size());
    return 1.0 - result;
}

QList<double> QualityMeasures::computeT(const QList<Measures> &measures,
										const Quantifier &quantifier,
										const QList<const BasicFuzzySet *> &qualifiers,
										const QList<const BasicFuzzySet *> &summarizers,
										const QList<QVector<QVariant> > &dbRows)
{
	QList<double> result;
	result.reserve(measures.size());
	for (int i = 0; i < measures.size(); i++) {
		double t;
		switch (measures.at(i)) {
			case T1:
				t = computeT1(quantifier, qualifiers, summarizers, dbRows);
				break;
			case T2:
				t = computeT2T9(summarizers, dbRows);
				break;
			case T3:
				t = computeT3(qualifiers, summarizers, dbRows);
				break;
			case T4:
				t = computeT4(qualifiers, summarizers, dbRows);
				break;
			case T5:
				t = computeT5T11(summarizers.size());
				break;
			case T6:
				t = computeT6(quantifier, dbRows.size());
				break;
			case T7:
				t = computeT7(quantifier, dbRows.size());
				break;
			case T8:
				t = computeT8T10(summarizers, dbRows);
				break;
			case T9:
				t = computeT2T9(qualifiers, dbRows);
				break;
			case T10:
				t = computeT8T10(qualifiers, dbRows);
				break;
			case T11:
				t = computeT5T11(qualifiers.size());
				break;
		}
		result.append(t);
	}
	return result;
}
