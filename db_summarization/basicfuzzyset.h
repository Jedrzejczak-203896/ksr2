#ifndef BASICFUZZYSET_H
#define BASICFUZZYSET_H

#include "membershipfuncineterface.h"
#include "fuzzyset.h"
#include<QVariant>
#include<QList>

class BasicFuzzySet : public FuzzySet
{
private:
	const MembershipFuncInterface *membershipFunction;
    int colNum;
public:
    BasicFuzzySet();
	double membership(const QVariant &element) const;
	double membership(const QVector<QVariant> &elements) const;
	const MembershipFuncInterface *getMembershipFunction() const;
	void setMembershipFunction(const MembershipFuncInterface *membershipFunction);

    double getColNum() const;
    void setColNum(const int colNum);
};

#endif // BASICFUZZYSET_H
