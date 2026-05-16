/*
* Module that houses all the nerdy math functions and objects.
*/
#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <string>

// Pi. Because why not?
const double pi = 3.1415926535897932384626433832795;

/*
* fact - Takes an integer x and returns x!
*/
double fact(int x) {
    double final = 1;
    for (int i = 1; i <= x; i++)
        final *= i;
    return final;
}

/*
* pascal - Takes two positive integers and returns the corresponding number in pascals triangle(r means row, n means column)
* Output is defined as r!/(n! * (r - n)!)
*/
double pascal(int r, int n) {
    return fact(r) / (fact(n) * fact(r - n));
}

/*
* equal(double) - Alternative check for equivalence if the difference between two given numbers is less than 10^-6.
*/
bool equal(double x, double y) {
    return (log10(abs(x - y)) < -6);
}

/*
* poly - Represents a polynomial.
*/
struct poly {
    // c - Represents the coefficients of the polynomial. The size of this vector is one more than the degree of the polynomial.
    // Example: A vector of {2,5,-7} would represent the polynomial of 2x^2 + 5x - 7.
    std::vector<double> c;

    poly() {}

    // Constructor that takes a double vector for the coefficients. A check for traling zeros is built in.
    poly(std::vector<double> co) : c(co) {
        int tZero = 0; // Tndicator for how many trailing zeros there might be.
        while (tZero < deg()+1 && !c[tZero]) // While loop runs until there is a nonzero coefficient or all the coefficients have been checked.
            tZero++;
        c.erase(c.begin(), c.begin() + tZero); // Any trailing zeros are erased.
    }

    // Operator overload () - Returns the function of x.
    // In the original example at the defintion of co, a function of 3 would be written as poly(3), and return 2(3)^2 + 5(3) - 7, or 26.
    double operator()(double x) {
        double final = 0;
        for (unsigned int i = 0; i < deg()+1; i++)
            final += pow(x, deg()+1 - 1 - i) * c[i]; // When i is 0, the value x is raised to the highest power, and that power decreases with each iteration until it reaches 0. Each product is added to the final return value.
        return final;
    }

    // Operator overload + - Returns the sum of the current poly object and poly object n as a new poly object.
    poly operator+(poly n){
        poly final = poly(c);
        for(int i=std::max(deg()+1,n.deg()+1)-std::min(deg()+1,n.deg()+1);i<std::max(deg()+1,n.deg()+1);i++)
            final.c[i] += n.c[i-std::max(deg()+1,n.deg()+1)+std::min(deg()+1,n.deg()+1)];
        return final;
    }

    // Operator overload += - Finds the sum of the current poly object and poly object n and sets the current poly object equal to it.
    void operator+=(poly n){*this = (*this+n);}

    // Operator overload - - Returns the difference of the current poly object and poly object n as a new poly object.
    poly operator-(poly n){
        poly final = poly(c);
        for(int i=std::max(deg()+1,n.deg()+1)-std::min(deg()+1,n.deg()+1);i<std::max(deg()+1,n.deg()+1);i++)
            final.c[i] -= n.c[i-std::max(deg()+1,n.deg()+1)+std::min(deg()+1,n.deg()+1)];
        return final;
    }

    // Operator overload -= - Finds the difference of the current poly object and poly object n and sets the current poly object equal to it.
    void operator-=(poly n){*this = (*this-n);}

    // Operator overload * - Returns the product of the current poly object and poly object n as a new poly object.
    poly operator*(poly n){
        std::vector<double> final;
        for (unsigned int i = 0; i < deg()+1; i++) {
            for (unsigned int j = 0; j < n.deg()+1; j++) {
                if (final.size() == i + j) // If the vector needs a new coefficient, it's appended. Otherwise, it's added on.
                    final.push_back(c[i] * n.c[j]);
                else
                    final[i + j] += c[i] * n.c[j];
            }
        }
        return poly(final); // final result is returned
    }

    // Operator overload *= - Finds the product of the current poly object and poly object n and sets the current poly object equal to it.
    void operator*=(poly n){*this = (*this*n);}

    // Operator overload == - Checks if current poly object is equal to poly object n.
    bool operator==(poly n){return c == n.c;}

    // Operator overload != - Checks if current poly object is not equal to poly object n.
    bool operator!=(poly n){return !(c == n.c);}

    // deg - Returns the degree of polynomial(the length of the vector c - 1).
    int deg(){
        return c.size()-1;
    }

    // print - Prints each coefficient.
    void print() {
        for (unsigned int i = 0; i < deg()+1; i++)
            std::cout << c[i] << " ";
    }
    
    // horShift - Horizontally shifts the current polynomial by a value of x and returns the result. Substitutes the expression (x - [shift value]) and evaluates.
    poly horShift(double x){
        std::vector<double> final;
        for (unsigned int i = 0; i < deg()+1; i++) {
            poly sub = poly(std::vector<double>({ c[i] })); // Starter coefficient.
            poly sub2 = poly(std::vector<double>({ 1,-x })); // Expression in the form of x - [shift value].
            for (unsigned int j = 0; j < deg()+1 - i - 1; j++)
                sub *= sub2; // The starter coefficient is multiplied by the substitution expression raised to the ith power.
            for (unsigned int j = 0; j < sub.deg()+1; j++) {
                if (!i)
                    final.push_back(sub.c[j]); // For the first iteration, each value is appended.
                else
                    final[i + j] += sub.c[j]; // For every iteration afterwards, the values are added on.
            }
        }
        return poly(final);
    }

    // slice - Slices the current polynomial by remapping 0 to s and 1 to e. Returns the result afterward
    poly slice(double s, double e){
        poly final = horShift(-s); // Starts off with a horizontal shift to the new start(defined as s)
        for (unsigned int i = 0; i < deg()+1; i++)
            final.c[i] *= pow(e - s, deg()+1 - 1 - i); // Each coefficient is multiplied by the difference between the new end and new start raised to its respective power.
        return final;
    }

    //Operator overload ^ - Raises the current poly object to the nth power and returns the result.
    poly operator^(int n){
        poly final=(*this);
        for(int i=1;i<n;i++)
            final *= (*this);
        return final;
    }

    //Operator overload ^= - Finds the current poly object to the nth power and sets the current poly object equal to it.
    void operator^=(int n){
        for(int i=1;i<n;i++)
            (*this) *= (*this);
    }

    //derive - Returns the derivative of the current poly object.
    poly derive(){
        std::vector<double> final;
        for (int i = 0; i < deg(); i++) // Derivative of a polynomial has a degree thats 1 less than its original.
            final.push_back(c[i] * (deg() - i)); // The value multiplied by the power is applied to the vector.
        return poly(final);
    }
};

/*
* expNodeTypes - Different types of expression nodes.
* POLYNOMIAL - Represents a poly object.
* COMPOUND - Represents a combination of previously calculated values.
* FRACTION - Represents two previously calculated values to be divided.
* CONSTANT - Represents a constant value.
*/
enum expNodeType {
    POLYNOMIAL,
    COMPOUND,
    FRACTION,
    CONSTANT,
};
/*
* fullNodeFuncNames - Names of each type of expression Node as strings.
*/
std::vector<std::string> expNodeNames = std::vector<std::string>({ "Polynomial","Compound","Fraction","Constant" });

/*
* fullNodeFuncType - Different types of expression node functions.
* NONE_FUNC - No function applied.
* SQRT - After value has been evaluated, find the square root of it and save it in its place.
* SIN - After value has been evaluated, find the sine of it and save it in its place.
* COS - After value has been evaluated, find the cosine of it and save it in its place.
*/
enum expNodeFuncType {
    NONE_FUNC,
    SQRT,
    SIN,
    COS,
};

/*
* fullNodeFuncNames - Names of each expression node function as strings.
*/
std::vector<std::string> expNodeFuncNames = std::vector<std::string>({ "None","Square root","Sine","Cosine" });

/*
* expNode - Represents a node in an exp object.
*/
struct expNode {
    // type - Type of the expNode.
    expNodeType type{};

    // val0 - Poly object for type POLYNOMIAL.
    poly val0{};

    // val1 - Int matrix for type COMPOUND.
    std::vector<std::vector<int>> val1{};

    // val2 - Int vector for type FRACTION.
    std::vector<int> val2{};

    // val3 - Double for type CONSTANT.
    double val3{};

    // f - Type of function thats applied to the expNode(SQRT, SIN, or COS).
    expNodeFuncType f{};

    // n - Dictates if the final value of the expNode is negated.
    bool n{};

    expNode() {}

    // zeroHandle - Basic function in case any faulty data is passed to either the POLYNOMIAL, COMPOUND, or FRACTION constructors.
    void zeroHandle(){
        type = CONSTANT;
        val3 = 0;
        if(f == COS)
            val3 = 1;
        val3 *= pow(-1,n);
    }

    // oneHandle - Basic function in case the value of any COMPOUND or FRACTION types is equal to 1.
    void oneHandle(){
        type = CONSTANT;
        val3 = 1;
        if(f == SIN)
            val3 = 1;
        else if(f == COS)
            val3 = 1;
        val3 *= pow(-1,n);
    }

    // If the constructor uses a poly object, then the type is set to POLYNOMIAL, with the function and negation boolean added on.
    // In cases where the poly object passed has no values in its vector, zeroHandle is called.
    // In cases where the poly object passed has only one value in its vector, the type is then set to CONSTANT with the only value in the poly object set as val3 for the expNode.
    expNode(poly x, expNodeFuncType fu, bool ne) {
        if(x.deg() == -1)
            zeroHandle();
        else if (!x.deg()) {
            if(!x.c[0])
                zeroHandle();
            else{
                type = CONSTANT;
                val3 = x.c[0];
                switch (fu) {
                case SQRT:
                    if (x.c[0] >= 0)
                        val3 = sqrt(x.c[0]);
                    else
                        val3 = 0; // Error case for when val3 is negative but has a SQRT function type.
                    break;
                case SIN:
                    val3 = sin(x.c[0]);
                    break;
                case COS:
                    val3 = cos(x.c[0]);
                    break;
                }
                val3 *= pow(-1, ne);
            }
        }
        else {
            type = POLYNOMIAL;
            val0 = x;
            f = fu;
            n = ne;
        }
    }

    // If the constructor uses a vector of int vectors, then the type is set to COMPOUND, with the function and negation boolean added on.
    // Any empty vectors within the passed matrix are discarded. If the whole matrix is empty, zeroHandle is called.
    expNode(std::vector<std::vector<int>> x, expNodeFuncType fu, bool ne) : type(COMPOUND), val1(x), f(fu), n(ne) {
        for (int i = val1.size() - 1; i > -1; i--) {
            if (!val1[i].size())
                val1.erase(val1.begin() + i, val1.begin() + i + 1);
        }
        if(!val1.size())
            zeroHandle();
    }

    // If the constructor uses an int vector, then the type is set to FRACTION, with the function and negation boolean added on.
    // If the vector passed does not have length of 2, zeroHandle is called.
    expNode(std::vector<int> x, expNodeFuncType fu, bool ne) : type(FRACTION), val2(x), f(fu), n(ne) {
        if(x.size()!=2)
            zeroHandle();
    }

    // If the constructor uses a double, then the type is set to CONSTANT. The function and negation boolean arent needed.
    expNode(double x) : type(CONSTANT), val3(x) {}

    // print - Prints all data for the expNode.
    void print() {
        switch (type) {
        case POLYNOMIAL:
            val0.print(); // Built-in print method is used for poly objects.
            break;
        case COMPOUND: // For COMPOUND types, each value within a vector is printed with a comma in between them. Every vector is separated by a bar.
                       // Example: For a matrix [[1, 2], [3, 4, 5]], the string "1, 2 | 3, 4, 5" would get printed to the console.
            for (unsigned int i = 0; i < val1.size(); i++) {
                for (unsigned int j = 0; j < val1[i].size() - 1; j++)
                    std::cout << val1[i][j] << ", ";
                if (val1[i].size()) // if the vector has values in it
                    std::cout << val1[i][val1[i].size() - 1];
                if (i < val1.size() - 1)
                    std::cout << " | ";
            }
            break;
        case FRACTION:
            std::cout << val2[0] << " " << val2[1]; // FRACTION types print their numerator and denominator values.
            break;
        case CONSTANT:
            std::cout << val3;
            break;
        }
        if (type != CONSTANT)
            std::cout << " [" << expNodeFuncNames[int(f)] << ", " << (n ? "Negated" : "Not negated") << "]"; // For all types except CONSTANT, function type and negation are printed.
    }

    // Operator overload == - Checks if expNode x is equal to the current expNode.
    bool operator==(expNode x){
        bool final=false;
        if (type == x.type && f == x.f && n == x.n) {
            switch (type) {
            case POLYNOMIAL:
                final = (val0 == x.val0);
                break;
            case COMPOUND:
                final = (val1 == x.val1);
                break;
            case FRACTION:
                final = (val2 == x.val2);
                break;
            case CONSTANT:
                final = (val3 == x.val3);
                break;
            }
        }
        return final;
    }

    // Operator overload != - Checks if expNode x is not equal to the current expNode.
    bool operator !=(expNode x){return !((*this) == x);}
};

/*
* critPoint - Represents a critical point in a full object(either a zero or an asymptote)
*/
struct critPoint {
    // point - Represents the value of the point.
    double point{};

    // isAsymp - Is true if the point specified is an asymptote. If false, the point is a zero.
    bool isAsymp{};

    critPoint() {}
    critPoint(double p, bool a) : point(p), isAsymp(a) {}
};

/*
* discOne - Represents a special label used during expression optimization.
* NEITHER - The expression node is neither to be discarded or equal to 1.
* IS_DISC - The expression node is to be discarded(it's value isn't used, is a constant equal to 0, or is a copy of an already existing value).
* IS_ONE - The expression node is equal to 1(is a constant equal to 1, or has identical values in a fraction).
*/
enum discOne {
    NEITHER,
    IS_DISC,
    IS_ONE
};

/*
* optNodeSpec - Represents a specification used during expression optimization.
*/
struct optNodeSpec {

    // d - Label for nodes that is used during optimization.
    discOne d = NEITHER;

    // dep - Is true if the correspoding node is dependent on the final answer. False if otherwise.
    bool dep = false;

    // copy - If the corresponding node is a copy of an already existing node, then this value will be the index of the original node. Otherwise, the node is unique, and the value will remain as -1.
    int copy = -1;
    optNodeSpec() {}
};

/*
* has(int) - Return true if n is in int vector l, false if otherwise.
*/
bool has(std::vector<int> l, int n) {
    bool found = false;
    int count = 0;
    while (!found && count < l.size()) { // While loop runs until either the value is found or every value is checked.
        found = (l[count] == n);
        count++;
    }
    return found;
}

/*
* has(double) - Similar to has(int), but uses double and the modified equivalence check.
*/
bool has(std::vector<double> l, double n) {
    bool found = false;
    int count = 0;
    while (!found && count < l.size()) {
        found = equal(l[count], n);
        count++;
    }
    return found;
}

/*
* exp - Represents a full expression using mathematical functions(square root, sin, cosine) and different combinations of them.
*/
struct expr {

    // l - The main vector of expNodes that make up the values used for evaluation.
    std::vector<expNode> l;

    // critPoints - Vector the represents any zeros or asymptotes the expression may have(typically within the range of 0 <= x <= 1).
    std::vector<critPoint> critPoints;

    // Constructors taken a list of expNodes, a single poly object(with or without function specified), or a constant.
    expr() {}
    expr(std::vector<expNode> li) : l(li) {}
    expr(poly p){
        if(!p.deg())
            l.push_back(expNode(p.c[0]));
        else
            l.push_back(expNode(p,NONE_FUNC,false));
    }
    expr(poly p, expNodeFuncType f, bool n){l.push_back(expNode(p,f,n));}
    expr(double n){l.push_back(expNode(n));}

    // f - Takes a value x and return the function of x.
    double operator()(double x) {
        std::vector<double> valStore; // Vector that stores values during evaluation. For each expNode, the values are evaluated and stored for future use
        for (unsigned int i = 0; i < len(); i++) {
            double cVal = 0;
            switch (l[i].type) {
            case POLYNOMIAL:
                cVal  = l[i].val0(x); // Current value equals the polynomial evaluated at value x.
                break;
            case COMPOUND: // For COMPOUND types, every value in the matrix refers to an index in valStore. For every vector, the valStore value found at each index is multiplied. Every product is then added to the current value.
                           // Example: For the matrix [[1, 2], [3, 4, 5]], valStore indicies 1 and 2 would be multiplied and added to the product of valStore indices 3, 4, and 5.
                for (unsigned int j = 0; j < l[i].val1.size(); j++) {
                    double vProd = 1;
                    for (unsigned int k = 0; k < l[i].val1[j].size(); k++)
                        vProd *= valStore[l[i].val1[j][k]];
                    cVal += vProd;
                }
                break;
            case FRACTION:
                cVal = valStore[l[i].val2[0]] / valStore[l[i].val2[1]]; // For FRACTION types, current value is set to the quotient of the specified valStore indicies.
                break;
            case CONSTANT:
                cVal = l[i].val3;
                break;
            }
            switch (l[i].f) {
            case SQRT:
                if(cVal >= 0)
                    cVal = sqrt(cVal);
                else
                    cVal = 0; // Error case for when the current value is less than 0 but has a SQRT function attached to it.
                break;
            case SIN:
                cVal = sin(cVal);
                break;
            case COS:
                cVal = cos(cVal);
                break;
            }
            cVal *= pow(-1, l[i].n);
            valStore.push_back(cVal); // For each value evaluated, the value is stored in valStore.
        }
        if (l.size()) // Upon successful evaluation, valStore will have the same amount of entries as l. The final value in this list will be the final value of the evaluated expression. In cases where l is empty, 0 is returned.
            return valStore[l.size() - 1];
        else
            return 0;
    }

    // push - Takes an expNode object and pushes it to the main list l.
    void push(expNode x) { // For COMPOUND and FRACTION types, the values they refer to are list indicies. Because of this, a special feature is used when the index in the passed node is less than 0, such that values at the end of the list can be more easily reached.
                           // If an expNode has an index of -1, the length of the original node list is added on to the index, such that the last value in the node list(at the time) is refered to.
                           // The same logic applies for values of -2 and -3, such that the second-to-last and third-to-last values are referred to, and so on and so forth.
                           // Example: If an expNode has an index value of -1 and is pushed to an exp object whose node list is of length 10, then that index of the expNode becomes -1+10, or 9, referring to the then last value of the expNode in the list.
        if (x.type == COMPOUND) {
            for (unsigned int i = 0; i < x.val1.size(); i++) {
                for (unsigned int j = 0; j < x.val1[i].size(); j++) {
                    if (x.val1[i][j] < 0)
                        x.val1[i][j] += int(len());
                }
            }
        }
        else if (x.type == FRACTION) {
            for (unsigned int i = 0; i < 2; i++) {
                if (x.val2[i] < 0)
                    x.val2[i] += int(len());
            }
        }
        l.push_back(x);
    }

    // len - Returns the length of the main list l.
    int len(){
        return l.size();
    }

    // print - Prints the contents of each expNode.
    void print() {
        for (unsigned int i = 0; i < len(); i++) {
            std::cout << i << " {" << expNodeNames[l[i].type] << "} ";
            l[i].print();
            std::cout << std::endl;
        }
    }

    // attach - Takes another exp object and attaches it to the current exp object.
    void attach(expr x) {
        int cLen = len();
        std::vector<std::vector<int>> cSamp; // Sample value for COMPOUND types.
        std::vector<int> fSamp; // Sample value for FRACTION types.
        for (unsigned int i = 0; i < x.len(); i++) { // For COMPOUND and FRACTION types, the indicies stored must be added by the length of the current list, otherwise they may refer to incorrect values during evaluation.
            if (x.l[i].type == COMPOUND) {
                cSamp.clear();
                for (unsigned int j = 0; j < x.l[i].val1.size(); j++) {
                    cSamp.push_back(std::vector<int>());
                    for (unsigned int k = 0; k < x.l[i].val1[j].size(); k++)
                        cSamp[cSamp.size() - 1].push_back(x.l[i].val1[j][k] + cLen);
                }
                l.push_back(expNode(cSamp, x.l[i].f, x.l[i].n)); // new compound node is applied
            }
            else if (x.l[i].type == FRACTION) {
                fSamp.clear();
                for (unsigned int j = 0; j < 2; j++)
                    fSamp.push_back(x.l[i].val2[j] + cLen);
                l.push_back(expNode(fSamp, x.l[i].f, x.l[i].n)); 
            }
            else
                l.push_back(x.l[i]);
        }
    }

    // copy - Returns a copy of the current expr object.
    expr copy() {
        expr final;
        for (unsigned int i = 0; i < len(); i++) {
            switch (l[i].type) {
            case 0:
                final.push(expNode(l[i].val0, l[i].f, l[i].n));
                break;
            case 1:
                final.push(expNode(l[i].val1, l[i].f, l[i].n));
                break;
            case 2:
                final.push(expNode(l[i].val2, l[i].f, l[i].n));
                break;
            case 3:
                final.push(expNode(l[i].val3));
                break;
            }
        }
        final.critPoints = critPoints;
        return final;
    }

    // Operator overload + - Returns the sum of the current expr object and expr object n as a new expr object.
    expr operator+(expr n){
        expr final = copy();
        int addInd = final.len()-1;
        final.attach(n);
        final.push(expNode(std::vector<std::vector<int>>({std::vector<int>({addInd}),std::vector<int>({-1})}),NONE_FUNC,false));
        return final;
    }

    // Operator overload += - Finds the sum of the current expr object and expr object n and sets the current expr object equal to it.
    void operator+=(expr n){*this = (*this+n);}

    // Operator overload - - Returns the differece of the current expr object and expr object n as a new expr object.
    expr operator-(expr n){
        expr final = copy();
        int subInd = final.len()-1;
        final.attach(n);
        if(final.l[final.len()-1].type == CONSTANT)
            final.l[final.len()-1].val3 *= -1;
        else
            final.l[final.len()-1].n = !final.l[final.len()-1].n;
        final.push(expNode(std::vector<std::vector<int>>({std::vector<int>({subInd}),std::vector<int>({-1})}),NONE_FUNC,false));
        return final;
    }

    // Operator overload -= - Finds the difference of the current expr object and expr object n and sets the current expr object equal to it.
    void operator-=(expr n){*this = (*this-n);}

    // Operator overload * - Returns the product of the current expr object and expr object n as a new expr object.
    expr operator*(expr n){
        expr final = copy();
        int multInd = final.len()-1;
        final.attach(n);
        final.push(expNode(std::vector<std::vector<int>>({std::vector<int>({multInd,-1})}),NONE_FUNC,false));
        return final;
    }

    // Operator overload *= - Finds the product of the current expr object and expr object n and sets the current expr object equal to it.
    void operator*=(expr n){*this = (*this*n);}

    // Operator overload / - Returns the quotient of the current expr object and expr object n as a new expr object.
    expr operator/(expr n){
        expr final = copy();
        int divInd = final.len()-1;
        final.attach(n);
        final.push(expNode(std::vector<int>({divInd,-1}),NONE_FUNC,false));
        return final;
    }

    // Operator overload /= - Finds the quotient of the current expr object and expr object n and sets the current expr object equal to it.
    void operator/=(expr n){*this = (*this/n);}

    //Operator overload ^ - Raises the current expr object to the nth power and returns the result.
    expr operator^(int n){
        expr final=(*this);
        for(int i=1;i<n;i++)
            final *= (*this);
        return final;
    }

    //Operator overload ^= - Finds the current expr object to the nth power and sets the current expr object equal to it.
    void operator^=(int n){
        for(int i=1;i<n;i++)
            (*this) *= (*this);
    }

    // optimize - Removes any unecessary nodes from the vector(copied values, zero/one constants, etc.)
    void optimize() {
        if (!l.size()) // Function ends if there are either 1) no values in the node list, or 2) the last value is either of type POLYNOMIAL or CONSTANT.
            return;
        if (l[l.size() - 1].type == POLYNOMIAL || l[l.size() - 1].type == CONSTANT) {
            l.erase(l.begin(), l.begin() + (l.size() - 1));
            return;
        }
        bool run = true; // Boolean to ensure that the main loop runs enough times until expression is fully optimized.
        while (run) {
            std::vector<int> uniques; // Vector that will store any node that isn't a copy of an already existing node.
            std::vector<optNodeSpec> nodeSpec; // Vector parallel to the main list l that stores optimization data for every node(if it's a zero, one, copy, etc.).
            for (unsigned int i = 0; i < l.size(); i++) {
                nodeSpec.push_back(optNodeSpec());
                discOne sDiscOne = NEITHER; // Sample discOne value. Every node starts off as NEITHER until proven otherwise.
                nodeSpec[i].dep = false; // Every node starts off as not dependent until proven otherwise. The proving algorithm is used later on.
                switch (l[i].type) {
                case POLYNOMIAL: // For POLYNOMIAL types, if there are no values in the poly object's vector, the initial value is 0 and function-based calculations are made to the determine the final value.
                    if (!l[i].val0.c.size()) {
                        if (l[i].f == COS) {
                            if (l[i].n)
                                l[i] = expNode(-1);
                            else
                                sDiscOne = IS_ONE; // The cosine of 0 is 1, so the sample discOne value becomes IS_ONE.
                        }
                        else
                            sDiscOne = IS_DISC;  // The sine and square root of 0 is 0, so the sample discOne value becomes IS_DISC(to be discarded).
                    }
                    break;
                case COMPOUND:
                    if (!l[i].val1.size()) {
                        if (l[i].f == COS) {
                            if (l[i].n)
                                l[i] = expNode(-1);
                            else
                                sDiscOne = IS_ONE;
                        }
                        else
                            sDiscOne = IS_DISC;
                    }
                    break;
                case FRACTION:
                    if (nodeSpec[l[i].val2[0]].d == IS_DISC || nodeSpec[l[i].val2[1]].d == IS_DISC) // If either component of the fraction is equal to 0, then the whole fraction is discarded.
                        sDiscOne = IS_DISC;
                    else {
                        if (l[i].val2[0] == l[i].val2[1]) { // Anything divided by itself is 1, so the node's oneHandle is called. If the resulting constant is 1, the node is labelled as IS_ONE.
                            l[i].oneHandle();
                            if (l[i].val3 == 1)
                                sDiscOne = IS_ONE;
                        }
                    }
                    break;
                case CONSTANT:
                    if (equal(l[i].val3, 0))
                        sDiscOne = IS_DISC;
                    else if (equal(l[i].val3, 1))
                        sDiscOne = IS_ONE;
                    break;
                }
                // Detection algorithm for copies and uniques. All nodes are cross referenced with the uniques vector.
                // If a node is equal to any unique node, its copy value because the unique node's index. If if doesn't have a copy, it gets added to the uniques vector.
                int copy = -1;
                int j = 0;
                while (copy == -1 && j < uniques.size()) {
                    if(l[uniques[j]] == l[i])
                        copy = uniques[j];
                    j++;
                }
                if (copy == -1)
                    uniques.push_back(int(i));
                nodeSpec[i].copy = copy;
                if (l[i].type == COMPOUND) { // For COMPOUND types, if the matrix has only one value, the node is labelled as a copy and is stripped of its unique status.
                    if (l[i].val1.size() == 1 && l[i].f == NONE_FUNC && !l[i].n) {
                        if (l[i].val1[0].size() == 1) {
                            if (nodeSpec[l[i].val1[0][0]].copy > -1)
                                nodeSpec[i].copy = nodeSpec[l[i].val1[0][0]].copy;
                            else
                                nodeSpec[i].copy = l[i].val1[0][0];
                        }
                    }
                    // Any index that is a copy get remapped to its respective unique index.
                    // There are some cases where an index for either a discarded value or a one may be present in a matrix.
                    // If a zero index is present in a matrix, then the entire vector is removed(anything times 0 is 0). If a one index is present in a matrix, then only that index is removed(anything times 1 is itself).
                    // If all the vectors get removed from the matrix at the end of the optimization, then the node is labelled as IS_DISC.
                    std::vector<std::vector<int>> sMat;
                    for (unsigned int j = 0; j < l[i].val1.size(); j++) {
                        std::vector<int> sVec;
                        bool isZero = false;
                        int k = 0;
                        while (k < l[i].val1[j].size() && !isZero) {
                            isZero = (nodeSpec[l[i].val1[j][k]].d == IS_DISC);
                            if (!isZero) {
                                int newValue = 0;
                                if (nodeSpec[l[i].val1[j][k]].copy > -1)
                                    newValue = nodeSpec[l[i].val1[j][k]].copy;
                                else
                                    newValue = l[i].val1[j][k];
                                if ((nodeSpec[l[i].val1[j][k]].d == IS_ONE && l[i].val1[j].size() == 1) || nodeSpec[l[i].val1[j][k]].d != IS_ONE)
                                    sVec.push_back(newValue);
                            }
                            k++;
                        }
                        if (!isZero)
                            sMat.push_back(sVec);
                    }
                    if (!sMat.size())
                        sDiscOne = IS_DISC;
                    l[i] = expNode(sMat, l[i].f, l[i].n);
                }
                else if (l[i].type == FRACTION) { // Similar logic is applied to FRACTION types to determine if they are copies. Since anything divided by one is itself, the node is a copy if it has a denominator index labelled as IS_ONE.
                    if (l[i].f == NONE_FUNC && !l[i].n) {
                        if (nodeSpec[l[i].val2[1]].d == IS_ONE) {
                            if (nodeSpec[l[i].val2[0]].copy > -1)
                                nodeSpec[i].copy = nodeSpec[l[i].val2[0]].copy;
                            else
                                nodeSpec[i].copy = l[i].val2[0];
                        }
                    }
                    std::vector<int> sFrac; // Any index that is a copy get remapped to its respective unique index.
                    for (unsigned int j = 0; j < 2; j++) {
                        if (nodeSpec[l[i].val2[j]].copy > -1)
                            sFrac.push_back(nodeSpec[l[i].val2[j]].copy);
                        else
                            sFrac.push_back(l[i].val2[j]);
                    }
                    if (nodeSpec[l[i].val2[0]].d == IS_DISC || nodeSpec[l[i].val2[1]].d == IS_DISC) // If either component of the fraction is equal to 0, then the whole fraction is discarded.
                        sDiscOne = IS_DISC;
                    else {
                        if (l[i].val2[0] == l[i].val2[1]) { // If both indicies of the fraction are equal to each other(after copy remapping), then oneHandle is called.
                            l[i].oneHandle();
                            if (l[i].val3 == 1)
                                sDiscOne = IS_ONE;
                        }
                        else
                            l[i] = expNode(sFrac, l[i].f, l[i].n);
                    }
                }
                nodeSpec[i].d = sDiscOne; // The sample discOne value is applied to every node after analysis.
            }
            if (nodeSpec[l.size() - 1].d == IS_DISC) {// If the final node is labelled IS_DISC, the entire node list is cleared and the function ends.
                l.clear();
                return;
            }
            else if (nodeSpec[l.size() - 1].d == IS_ONE) {// If the final node is labelled IS_ONE, then the list becomes only a CONSTANT with value 1 and the function ends.
                l = std::vector<expNode>({ expNode(1) });
                return;
            }
            else if (nodeSpec[l.size() - 1].copy > -1) {// Similar logic is applied here in the case of IS_DISC and IS_ONE final nodes.
                if (nodeSpec[nodeSpec[l.size() - 1].copy].d == IS_DISC) {
                    l.clear();
                    return;
                }
                else if (nodeSpec[nodeSpec[l.size() - 1].copy].d == IS_ONE) {
                    l = std::vector<expNode>({ expNode(1) });
                    return;
                }
                l.erase(l.begin() + nodeSpec[l.size() - 1].copy + 1, l.begin() + l.size()); // Assuming the final node is a copy of another node, anything past that unique index is erased in both the main list and the nodeSpec list.
                nodeSpec.erase(nodeSpec.begin() + nodeSpec[nodeSpec.size() - 1].copy + 1, nodeSpec.begin() + nodeSpec.size());
            }
            //Inspection algorithm to find out which nodes the final value is dependent on. 
            std::vector<int> inspect = std::vector<int>({int(l.size()) - 1}); // Used to determine what nodes are dependent.
            nodeSpec[l.size() - 1].dep = true; // The final node is dependent by default.
            while (inspect.size()) {
                // For as long as there are dependent nodes, the while loop will keep running. Indicies from either COMPOUND or FRACTION nodes are logged and inspected.
                // After the only nodes referenced by indicies are either POLYNOMIAL or CONSTANT types, the while loop will end and all the nodes that make up the final value will be labelled as such.
                if (l[inspect[0]].type == COMPOUND) {
                    for (unsigned int j = 0; j < l[inspect[0]].val1.size(); j++) {
                        for (unsigned int k = 0; k < l[inspect[0]].val1[j].size(); k++) {
                            if (!nodeSpec[l[inspect[0]].val1[j][k]].dep) {
                                nodeSpec[l[inspect[0]].val1[j][k]].dep = true;
                                inspect.push_back(l[inspect[0]].val1[j][k]);
                            }
                        }
                    }
                }
                else if (l[inspect[0]].type == FRACTION) {
                    for (unsigned int j = 0; j < 2; j++) {
                        if (!nodeSpec[l[inspect[0]].val2[j]].dep) {
                            nodeSpec[l[inspect[0]].val2[j]].dep = true;
                            inspect.push_back(l[inspect[0]].val2[j]);
                        }
                    }
                }
                inspect.erase(inspect.begin()); // The first value in the vector is erased and continues until there are no more.
            }
            // As useless values are removed from the list, a shift system is set up to ensure that the indicies of either COMPOUND or FRACTION types still refer to the proper values.
            // For every value thats removed, shiftVal is incremented. For every entry, shiftVal is appointed to the vector shift, and is equivalent to the amount of removed entries before the current index.
            int shiftVal = 0;
            std::vector<int> shift;
            run = false; // The main while loop is stopped until a value is removed from the list, signaling the need for another round of optimization.
            for (int i = 0; i < nodeSpec.size(); i++) {
                if (nodeSpec[i].dep) { // If the value is dependent, shifts occur for both COMPOUND and POLYNOMIAL types.
                    if (l[i - shiftVal].type == COMPOUND) {
                        for (unsigned int j = 0; j < l[i - shiftVal].val1.size(); j++) {
                            for (unsigned int k = 0; k < l[i - shiftVal].val1[j].size(); k++)
                                l[i - shiftVal].val1[j][k] -= shift[l[i - shiftVal].val1[j][k]];
                        }
                    }
                    if (l[i - shiftVal].type == FRACTION) {
                        for (unsigned int j = 0; j < 2; j++)
                            l[i - shiftVal].val2[j] -= shift[l[i - shiftVal].val2[j]];
                    }
                }
                else { // Erasing a useless value and marking the main while loop for another iteration.
                    l.erase(l.begin() + (i - shiftVal));
                    run = true;
                    shiftVal++;
                }
                shift.push_back(shiftVal);
            }
        }
    }
    
    // derive - Finds the derivative of the main expr object and returns it.
    expr derive() {
        // In order to calculate the derivative, the derivative of every value is calculated and treated the same a standard expr object.
        // The indicies for all derivative terms are stored in dTerms.
        expr final = copy();
        std::vector<std::vector<int>> sMat;
        expNode sNode;
        std::vector<int> dTerms;
        for (unsigned int i = 0; i < len(); i++) {
            switch (final.l[i].type) {
            case POLYNOMIAL: // For POLYNOMIAL types, the derivative of the polynomial is calculated and stored.
                final.push(expNode(final.l[i].val0.derive(), NONE_FUNC, false));
                break;
            case COMPOUND:
                // For COMPOUND types, the chain rule is used to find the derivative. Both normal terms and derivative terms are used alike.
                // Example: For terms a, b, c, whose derivatives are A, B, C, respectively, the derivative of the expression a*b*c is equal to A*b*c + a*B*c + a*b*C, which can easily be stored in a another COMPOUND node.
                sMat.clear();
                for (unsigned int j = 0; j < final.l[i].val1.size(); j++) {
                    for (unsigned int k = 0; k < final.l[i].val1[j].size(); k++) {
                        sMat.push_back(std::vector<int>());
                        for (unsigned int m = 0; m < final.l[i].val1[j].size(); m++) {
                            if (k == m)
                                sMat[sMat.size() - 1].push_back(dTerms[final.l[i].val1[j][m]]);
                            else
                                sMat[sMat.size() - 1].push_back(final.l[i].val1[j][m]);
                        }
                    }
                }
                final.push(expNode(sMat, NONE_FUNC, false));
                break;
            case FRACTION: // For FRACTION types, the chain rule is also used, such that the derivative of a/b is equal to (A*b-B*a)/b^2, which is calculated using both COMPOUND and FRACTION types.
                final.push(expNode(std::vector<std::vector<int>>({ std::vector<int>({ dTerms[final.l[i].val2[0]],final.l[i].val2[1] }) }), NONE_FUNC, false));
                final.push(expNode(std::vector<std::vector<int>>({ std::vector<int>({ final.l[i].val2[0],dTerms[final.l[i].val2[1]] }) }), NONE_FUNC, true));
                final.push(expNode(std::vector<std::vector<int>>({ std::vector<int>({ -2 }),std::vector<int>({ -1 }) }), NONE_FUNC, false));
                final.push(expNode(std::vector<std::vector<int>>({ std::vector<int>({ final.l[i].val2[1],final.l[i].val2[1] }) }), NONE_FUNC, false));
                final.push(expNode(std::vector<int>({ -2,-1 }), NONE_FUNC, false));
                break;
            case CONSTANT:
                final.push(expNode(0)); // The derivative of any constant is 0.
                break;
            }
            // Once the derivative of the original expression is calculated, a function type may alter it afterwards.
            switch (final.l[i].f) {
            case NONE_FUNC: // Ensuring negation remains the same.
                final.l[final.len()-1].n = final.l[i].n;
                break;
            case SQRT: // For SQRT functions, the derivative is evaluated as A/(2*sqrt(a)).
                final.push(expNode(2));
                final.push(expNode(std::vector<std::vector<int>>({ std::vector<int>({int(i),-1}) }), NONE_FUNC, false));
                final.push(expNode(std::vector<int>({ -3,-1 }), NONE_FUNC, false));
                break;
            case SIN: // For SIN functions, the derivative is evaluated as Acos(a).
                sNode = final.l[i];
                sNode.f = COS;
                final.push(sNode);
                final.push(expNode(std::vector<std::vector<int>>({ std::vector<int>({-2,-1}) }), NONE_FUNC, false));
                break;
            case COS: // For COS functions, the derivative is evaluated as -Asin(a).
                sNode = final.l[i];
                sNode.f = SIN;
                sNode.n = !sNode.n;
                final.push(sNode);
                final.push(expNode(std::vector<std::vector<int>>({ std::vector<int>({-2,-1}) }), NONE_FUNC, false));
                break;
            }
            dTerms.push_back(int(final.l.size()) - 1); // Every term is evaluated and stored. This final evaluated term is the derivative of the original expression.
        }
        final.optimize(); // Especially in the cases of multiple-order derivatives, many empty value/copies can be present. The optimization function greatly assists with such cases.
        return final;
    }

    // slice - Applies the slice() function to every POLYNOMIAL node and returns the result.
    expr slice(double s, double e) {
        expr final = copy();
        for (unsigned int i = 0; i < final.l.size(); i++) {
            if (final.l[i].type == POLYNOMIAL)
                final.l[i].val0 = final.l[i].val0.slice(s, e);
        }
        return final;
    }
};

/*
* sqrt - Takes an expr object and returns the square root of it.
*/
expr sqrt(expr x){
    expr final = x.copy();
    if(final.l[final.len()-1].f == NONE_FUNC)
        final.l[final.len()-1].f = SQRT;
    else
        final.push(expNode(std::vector<std::vector<int>>({std::vector<int>({-1})}),SQRT,false));
    return final;
}

/*
* sin - Takes an expr object and returns the sine of it.
*/
expr sin(expr x){
    expr final = x.copy();
    if(final.l[final.len()-1].f == NONE_FUNC)
        final.l[final.len()-1].f = SIN;
    else
        final.push(expNode(std::vector<std::vector<int>>({std::vector<int>({-1})}),SIN,false));
    return final;
}

/*
* cos - Takes an expr object and returns the cosine of it.
*/
expr cos(expr x){
    expr final = x.copy();
    if(final.l[final.len()-1].f == NONE_FUNC)
        final.l[final.len()-1].f = COS;
    else
        final.push(expNode(std::vector<std::vector<int>>({std::vector<int>({-1})}),COS,false));
    return final;
}


/*
* comp - Represents a composite expression such that given expr objects x and y, x(y) is returned.
*/
expr comp(expr x, expr y) {
    expr final = y.copy();
    int initInd = int(final.len()) - 1; // Represents the index of the evaluated value of the y expr object.
    std::vector<std::vector<int>> sMat;
    std::vector<int> sVec;
    // Whenever a POLYNOMIAL types coeffecients get evaluated in a comp function, they need to get added in one by one as constants so that they can be multiplied.
    // Because of this, a new vector is needed to store the new indicies that represent the old values.
    std::vector<int> newIndexes;
    for (unsigned int i = 0; i < x.len(); i++) {
        switch (x.l[i].type) {
        case POLYNOMIAL:
            sMat.clear();
            for (unsigned int j = 0; j < x.l[i].val0.c.size(); j++) {
                sMat.push_back(std::vector<int>());
                final.push(expNode(x.l[i].val0.c[j]));
                sMat[sMat.size() - 1].push_back(int(final.len()) - 1);
                for (unsigned int k = 0; k < x.l[i].val0.c.size() - j - 1; k++)
                    sMat[sMat.size() - 1].push_back(initInd);
            }
            final.push(expNode(sMat, x.l[i].f, x.l[i].n));
            break;
        case COMPOUND:
            sMat.clear();
            for (unsigned int j = 0; j < x.l[i].val1.size(); j++) {
                sMat.push_back(std::vector<int>());
                for (unsigned int k = 0; k < x.l[i].val1[j].size(); k++) {
                    sMat[sMat.size() - 1].push_back(newIndexes[x.l[i].val1[j][k]]);
                }
            }
            final.push(expNode(sMat, x.l[i].f, x.l[i].n));
            break;
        case FRACTION:
            sVec.clear();
            for (unsigned int j = 0; j < 2; j++)
                sVec.push_back(newIndexes[x.l[i].val2[j]]);
            final.push(expNode(sVec, x.l[i].f, x.l[i].n));
            break;
        case CONSTANT:
            final.push(expNode(x.l[i].val3));
            break;
        }
        newIndexes.push_back(int(final.len()) - 1);
    }
    final.optimize();
    return final;
}

// angleDeg - If set to true, the deriveAngle function of the point object will return value in degrees. Radians if otherwise.
bool angleDeg = false;

/*
* point - Represents an ordered pair with an x and y value.
*/
struct point {
    double x{};
    double y{};
    point() {}
    point(double xp, double yp) : x(xp), y(yp) {}

    // print - Prints x and y.
    void print() {
        std::cout << x << ", " << y;
    }

    // Operator overload == - Compares the x and y values using the modified equivalence check and returns true if both are equal.
    bool operator==(point n){
        return equal(x, n.x) && equal(y, n.y);
    }

    // Operator overload != - Compares the x and y values using the modified equivalence check and returns false if both are equal.
    bool operator!=(point n){
        return !((*this) == n);
    }

    // deriveAngle - Calculates the angle made by the vector between the point and the origin in standard notation. Returns in radians by default, but will return in degrees if angleDeg is set to true.
    double deriveAngle() {
        double final = 0;
        if (equal(x, 0)) {
            if (y > 0)
                final = pi/2;
            if (y < 0)
                final = 3*pi/2;
        }
        else{
            final = atan(y / x);
            if (x < 0)
                final += pi;
            if (x > 0 && y < 0)
                final += 2*pi;
        }
        if(angleDeg)
            final = final * 180 / pi;
        return final;
    }
};

/*
* dist - Calculates the distance between two points.
*/
double dist(point p1, point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
};

/*
* ps - Represents a point slope function in either the form y = mx + b or x = my + b.
*/
struct ps {
    bool yFunc{};
    double m{};
    double b{};
    ps() {}
    ps(point p1, point p2) {
        // To prevent slope divisions by 0, yFunc is set to true only if the difference in the x values of the reference points is greater than the y.
        // By doing this, the slope of any ps object will be less than 1.
        double difX = p2.x - p1.x;
        double difY = p2.y - p1.y;
        yFunc = (abs(difX) > abs(difY));
        if (yFunc) {
            m = difY / difX;
            b = -m * p1.x + p1.y;
        }
        else {
            m = difX / difY;
            b = -m * p1.y + p1.x;
        }
    }

    // Operator overload () - Finds the point that would lie on the line created by the ps object parameters.
    point operator()(double x) {
        if (yFunc)
            return point(x, m * x + b);
        else
            return point(m * x + b, x);
    }

    // print - Prints the slope, intercept, and if the function is of y or x.
    void print() {
        std::cout << m << " " << b << " " << yFunc;
    }
};

/*
* bounds - Represents a rectangular area with minimums and maximums.
*/
struct bounds {
    double minX{};
    double minY{};
    double maxX{};
    double maxY{};
    bounds() {}
    bounds(double ix, double iy, double ax, double ay) : minX(ix), minY(iy), maxX(ax), maxY(ay) {}

    // within - Checks if point p lies is greater than the minimums and less than the maximums.
    bool within(point p) {
        return (p.x > minX && p.x < maxX && p.y > minY && p.y < maxY);
    }

    // print - Prints the minimums and the maximums.
    void print() {
        std::cout << minX << " " << minY << " " << maxX << " " << maxY;
    }

    // diagonal - Returns the distance between the minimum point and the maximum point.
    double diagonal() {
        return sqrt(pow(maxX - minX, 2) + pow(maxY - minY, 2));
    }
};

/*
* linePixel - Represents a pixel in a line of a parametric curve.
*/
struct linePixel {
    int x{};
    int y{};

    // mag - Refers to how close the line is to the curve and how strongly the curves properties would be rendered in an image. Contained within the range 0 - 1.
    // Example: A linePixel with mag 0.5 that's part of a black(#000000) line on a white(#ffffff) background would render as gray(#808080, the color directly between black and white rounded to the nearest integer).
    double mag{};
    linePixel() {}
    linePixel(int xp, int yp, double m) : x(xp), y(yp), mag(m) {}

    // print - Prints the x, y, and mag values.
    void print() {
        std::cout << x << ", " << y << ", " << mag << "\n";
    }
};

/*
* transformType - Represents a type of transformation.
*/
enum transformType{
    TRANSLATE,
    ROTATE,
    SCALE,
    P_TRANSLATE,
    P_ROTATE,
    P_SCALE,
};

/*
* transformType - Represents a type of transformation.
*/
std::vector<std::string> transformNames = std::vector<std::string>({"Translate","Rotate","Scale","Progressive Translate","Progressive Rotate","Progressive Scale"});

/*
* transformation - Represents a transformation that can be given to a para object.
*/
struct transformNode {
    transformType type{};
    double val0{};
    double val1{};
    expr val2{};
    expr val3{};
    // If there are two double values, the type must be specifed because it could either mean a TRANSLATE or a SCALE. One double would a rotation around the origin using a single degree value.
    // The same logic applies for progressive transformations.
    transformNode(transformType t, double x, double y) : type(t), val0(x), val1(y) {}
    transformNode() {}
    transformNode(double x) : type(ROTATE), val0(x) {}
    transformNode(transformType t, expr x, expr y) : type(t), val2(x), val3(y) {}
    transformNode(expr x) : type(P_ROTATE), val2(x) {}

    // print - Prints type of node and subsequent data.
    void print() {
        std::cout << transformNames[int(type)] << " | ";
        if(type == TRANSLATE || type == SCALE)
            std::cout << "x: " << val0 << ", y: " << val1;
        else if(type == ROTATE)
            std::cout << "degrees: " << val0;
        else if(type == P_TRANSLATE || type == P_SCALE){
            std::cout << "\nx:\n";
            val2.print();
            std::cout << "\ny:\n";
        }
        else{
            std::cout << "\n";
            val2.print();
        }
    }
};

/*
* para - Represents a parametric curve with an x and y component.
*/
struct para {
    expr x{};
    expr y{};

    // configured - Set to true to avoid unnecessary reconfiguration of cached values.
    bool configured{};

    // positions, main/deriv/doubleDeriv points - Cached values to avoid constant recalculation. Configured once per curve.
    std::vector<double> positions;
    std::vector<point> mainPoints;
    std::vector<point> derivPoints;
    std::vector<point> doubleDerivPoints;

    // limits - the minimums and maximums of the curve.
    bounds limits;

    // length - Approximate length of the curve.
    double length;

    // start/end An - Initial and terminal angles of the direction vector of the curve(in radians).
    double startAn;
    double endAn;

    // linePixels - Pixel positions and magnitudes to be rendered for the curve in an image.
    std::vector<linePixel> linePixels;

    // Constructor can take either full expressions or poly objects.
    para() {}
    para(expr xe, expr ye) : x(xe), y(ye) {}
    para(poly xe, poly ye) : x(xe), y(ye) {}

    // Operator overload () - Finds the point that would lie on the curve at value n.
    point operator()(double n) {
        return point(x(n), y(n));
    }

    // derive - Derives each component of the curve and returns the result as a new para object.
    para derive(){
        return para(x.derive(),y.derive());
    }

    // anglePoint - Returns the angle of the direction vector of the parametric curve at point n
    double anglePoint(double n) {
        return derive()(n).deriveAngle();
    }

    // print - Prints x and y.
    void print() {
        std::cout << "x:\n";
        x.print();
        std::cout << "\ny:\n";
        y.print();
        std::cout << "\n" << (configured ? "Configured" : "Not configured");
        if(configured){
            std::cout << "\nPoint cache length: " << mainPoints.size() << "\nLimits: ";
            limits.print();
            std::cout << "\nLength: " << length << "\nStart angle: " << startAn << "\nEnd angle: " << endAn;
            if(linePixels.size())
                std::cout << "\nLine pixel count: " << linePixels.size();
        }
    }

    // printPoint - Prints the value of the parametric curve at point x.
    void printPoint(double x) {
        (*this)(x).print();
    }

    // ends - Prints the values of the parametric curve at 0 and 1.
    void ends() {
        printPoint(0);
        std::cout << std::endl;
        printPoint(1);
    }

    // optimize - Optimizes each component.
    void optimize() {
        x.optimize();
        y.optimize();
    }

    // tanLine - Finds the tangent line at point t by finding the value of the derivative onto the original and storing the result in a ps object.
    ps tanLine(double t) {
        para d = derive();
        point tp = (*this)(t);
        return ps(tp, point(d.x(t) + tp.x, d.y(t) + tp.y));
    };

    // angleDeriv - Finds the derivative of the function of the direction angle(arctangent of quotient of the derivatives of the y and x components) and stores it as a single expression
    expr angleDeriv() {
        para dc = derive();
        para ddc = dc.derive();
        return (ddc.y*dc.x-ddc.x*dc.y)/((dc.x^2)+(dc.y^2));
    }
    // slice - Slices the x and y compnents of the current para and returns the result.
    para slice(double s, double e) {
        return para(x.slice(s, e), y.slice(s, e));
    }

    // copy - Returns a copy of the current para.
    para copy(){
        return para(x.copy(),y.copy());
    }

    // configure - Configures all cached data for the curve.
    void configure() {
        if (!configured) {
            configured = true;
            // derivative and double derivative curves are initialized.
            para dc = derive();
            para ddc = dc.derive();
            double p = 0;
            point fp = (*this)(p);
            point dp = dc(p);
            point ddp = ddc(p);

            // A rough estimate of the curve limits is made before traversal.
            limits.minX = fp.x;
            limits.maxX = fp.x;
            limits.minY = fp.y;
            limits.maxY = fp.y;
            point limPoint;
            for (double i = 0; i <= 1; i += 0.1) {
                limPoint = (*this)(i);
                limits.minX = std::min(limPoint.x, limits.minX);
                limits.maxX = std::max(limPoint.x, limits.maxX);
                limits.minY = std::min(limPoint.y, limits.minY);
                limits.maxY = std::max(limPoint.y, limits.maxY);
            }

            // All cached value fall between the range 0 - 1, inclusive.
            while (p < 1) {
                if (mainPoints.size() > 0) { // Ensures that the current isn't equal to(or negligibly close to) the previous point. Curve limits are also updated progressively.
                    if (fp != mainPoints[mainPoints.size() - 1]) {
                        positions.push_back(p);
                        mainPoints.push_back(fp);
                        derivPoints.push_back(dp);
                        doubleDerivPoints.push_back(ddp);
                        limits.minX = std::min(fp.x, limits.minX);
                        limits.maxX = std::max(fp.x, limits.maxX);
                        limits.minY = std::min(fp.y, limits.minY);
                        limits.maxY = std::max(fp.y, limits.maxY);
                    }
                }
                else {
                    positions.push_back(p);
                    mainPoints.push_back(fp);
                    derivPoints.push_back(dp);
                    doubleDerivPoints.push_back(ddp);
                }

                // Approximation of the curve length. If I could properly integrate using this module, I would.
                if (mainPoints.size() > 1)
                    length += dist(mainPoints[mainPoints.size() - 2], mainPoints[mainPoints.size() - 1]);
                // Traversal is made primarily using the reciprocal of the magnitude of the direction vector. In cases where such magnitude approaches 0, the reciprocal of the diagonal of the curve limits is used instead.
                p += 1 / std::max(dist(dp, point(0, 0)), limits.diagonal());
                fp = (*this)(p);
                dp = dc(p);
                ddp = ddc(p);
            }
            positions.push_back(1);
            mainPoints.push_back((*this)(1));
            derivPoints.push_back(dc(1));
            doubleDerivPoints.push_back(ddc(1));
            length += dist(mainPoints[mainPoints.size() - 2], mainPoints[mainPoints.size() - 1]);

            // In cases where magnitude of the curve approaches 0 at either 0 or 1, an estimation algorithm is used to determine what the angle the curve approaches to.
            // Since the divisions between microscopic values can be unpredictable, traversal by 0.000001 is repeated until a value that isn't negligibly small is found to divide.
            double subS = 0.000001;
            double subE = 0.999999;
            point tpS = dc(subS);
            point tpE = dc(subE);
            if (derivPoints[0] == point(0, 0)) {
                while (tpS == point(0, 0)) {
                    subS += 0.000001;
                    tpS = dc(subS);
                }
                startAn = tpS.deriveAngle();
            }
            else
                startAn = derivPoints[0].deriveAngle();
            if (derivPoints[derivPoints.size() - 1] == point(0, 0)) {
                while (tpE == point(0, 0)) {
                    subE -= 0.000001;
                    tpE = dc(subE);
                }
                endAn = tpE.deriveAngle();
            }
            else
                endAn = derivPoints[derivPoints.size() - 1].deriveAngle();
        }
    }

    // findClosest - Finds the position on the line between 0 and 1 that's closest to point p. Returns the position and the distance.
    std::vector<double> findClosest(point p) {
        if (!configured) // Cached main points are used as a basis for estimation.
            configure();
        double final = 0;
        double lowestValue = dist(p, mainPoints[0]);
        int lowestIndex = 0;
        double sampleDist = 0;
        for (unsigned int i = 0; i < mainPoints.size(); i++) {
            sampleDist = dist(p, mainPoints[i]);
            if (sampleDist < lowestValue) {
                lowestValue = sampleDist;
                lowestIndex = i;
            }
        }
        final = positions[lowestIndex];
        double startPoint; // Refers to the power of 10 the estimation process should start at. Varies directly with the magnitude of the curve at the intially estimated closest point.
        if (pow(derivPoints[lowestIndex].x, 2) + pow(derivPoints[lowestIndex].y, 2) > 0)
            startPoint = log10(pow(derivPoints[lowestIndex].x, 2) + pow(derivPoints[lowestIndex].y, 2)) + 1;
        else
            startPoint = 3;
        for (int k = 0; k < 10; k++) { // An additional 10 decimal digits of accuracy are given to the estimation.
            for (int i = -10; i < 11; i++) {
                sampleDist = dist(p, (*this)(final + i / pow(10, k + startPoint)));
                if (i == -10) {
                    lowestIndex = -10;
                    lowestValue = sampleDist;
                }
                if (sampleDist <= lowestValue) {
                    lowestIndex = i;
                    lowestValue = sampleDist;
                }
            }
            final += lowestIndex / pow(10, k + startPoint); // If the value exceeds the bounds of 0 - 1, the estimation is stopped.
            if (final > 1) {
                final = 1;
                break;
            }
            if (final < 0) {
                final = 0;
                break;
            }
        }
        return std::vector<double>({final, dist(p, (*this)(final))});
    }

    // generatePixels - Finds any pixel that's close enough to the main curve to be rendered depending on a specified thickness.
    void generatePixels(double th) {
        if (!configured)
            configure();
        double p = 0;
        std::vector<linePixel> allFinal;
        para dc = derive();
        point dp;
        while (p < 1) { // Potential pixels are analyzed from positions 0 to 1.
            std::vector<point> potentials;
            point initP = (*this)(p);
            // The x and y values of the point at the current position are checked to see if they are close enough to whole numbers. The potential pixel positions are then extracted.
            std::vector<bool> whole = std::vector<bool>({ equal(initP.x - int(initP.x), 0), equal(initP.y - int(initP.y), 0) });
            if (whole[0] == whole[1]) {
                if (whole[0])
                    potentials.push_back(initP);
                else {
                    for (int i = 0; i < 4; i++)
                        potentials.push_back(point(floor(initP.x) + floor(i / 2), floor(initP.y) + int((floor((i + 1) / 2))) % 2));
                }
            }
            else {
                if (whole[0]) {
                    for (int i = 0; i < 2; i++)
                        potentials.push_back(point(initP.x, floor(initP.y) + i));
                }
                else {
                    for (int i = 0; i < 2; i++)
                        potentials.push_back(point(floor(initP.x) + i, initP.y));
                }
            }
            double highest = 0;
            std::vector<linePixel> closeFinal;
            for (int i = 0; i < potentials.size(); i++) {
                // For every potential point, an approximation of closest position is made and used to advance the traversal.
                // I don't even remember when I wrote this code. It would take a miracle to properly document this.
                dp = dc(p);
                double mag = (equal(sqrt(dp.x * dp.x + dp.y * dp.y), 0) ? limits.diagonal() : sqrt(dp.x * dp.x + dp.y * dp.y));
                std::vector<double> range = std::vector<double>({ std::max(double(0), p - 1 / mag), std::min(double(1), p + 1 / mag) });
                double fip = p;
                bool run = true;
                point lp;
                point cep;
                point hp;
                double ld;
                double ced;
                double hd;
                //if(potentials[i].x == 58 and potentials[i].y == 56)
                    //std::cout << p << "\n";
                while (run) {
                    lp = (*this)(range[0]);
                    cep = (*this)(fip);
                    hp = (*this)(range[1]);
                    ld = dist(lp, potentials[i]);
                    ced = dist(cep, potentials[i]);
                    hd = dist(hp, potentials[i]);
                    if (int(ld * 256) == int(ced * 256) || int(ced * 256) == int(hd * 256))
                        run = false;
                    if (ld < ced && ld < hd)
                        range[1] = fip;
                    else if (ced < ld && ced < hd) {
                        range[0] += (fip - range[0]) / 2;
                        range[1] += (fip - range[1]) / 2;
                    }
                    else if (hd < ld && hd < ced)
                        range[0] = fip;
                    fip = (range[0] + range[1]) / 2;
                }
                double fid = 0;
                lp = (*this)(range[0]);
                cep = (*this)(fip);
                hp = (*this)(range[1]);
                ld = dist(lp, potentials[i]);
                ced = dist(cep, potentials[i]);
                hd = dist(hp, potentials[i]);
                if (ld < ced && ld < hd) {
                    fip = range[0];
                    fid = ld;
                }
                else if (ced < ld && ced < hd)
                    fid = ced;
                else if (hd < ld && hd < ced) {
                    fip = range[1];
                    fid = hd;
                }
                std::vector<double> test = std::vector<double>({ fip, fid });
                if (test[1] < 1)
                    closeFinal.push_back(linePixel(int(potentials[i].x), int(potentials[i].y), test[1]));
                if (test[0] > highest && abs(test[0] - p) < 2 / mag)
                    highest = test[0];
            }
            if (highest == p) {
                dp = dc(p);
                highest = p + 1 / (equal(sqrt(dp.x * dp.x + dp.y * dp.y), 0) ? limits.diagonal() : sqrt(dp.x * dp.x + dp.y * dp.y));
                if (highest > 1)
                    highest = 1;
            }
            for (int i = 0; i < closeFinal.size(); i++)
                allFinal.push_back(closeFinal[i]);
            p = highest;
        }
        std::vector<linePixel> extFinal; // For every potential point, an extension based off thickness is used to capture all necessary points.
        for (int i = 0; i < allFinal.size(); i++) {
            extFinal.push_back(allFinal[i]);
            for (int j = -ceil((th - 1) / 2); j < ceil((th - 1) / 2) + 1; j++) {
                for (int k = -ceil((th - 1) / 2); k < ceil((th - 1) / 2) + 1; k++) {
                    if (!(j == 0 && k == 0))
                        extFinal.push_back(linePixel(allFinal[i].x - j, allFinal[i].y - k, -1));
                }
            }
        }
        std::vector<linePixel> filteredFinal; // For every point, its closest distance to the main curve is found and stored as magnitude.
        for (int i = 0; i < extFinal.size(); i++) {
            bool found = false;
            int j = 0;
            while (j < filteredFinal.size() && !found) {
                if (extFinal[i].x == filteredFinal[j].x && extFinal[i].y == filteredFinal[j].y)
                    found = true;
                j++;
            }
            if (!found) {
                if (extFinal[i].mag == -1)
                    extFinal[i].mag = findClosest(point(extFinal[i].x, extFinal[i].y))[1];
                extFinal[i].mag -= (th - 1) / 2;
                extFinal[i].mag = std::max(double(0), extFinal[i].mag);
                if (extFinal[i].mag <= 1){
                    extFinal[i].mag = 1-extFinal[i].mag;
                    filteredFinal.push_back(extFinal[i]);
                }
            }
        }
        linePixels = filteredFinal;
    }

    // transform(singular) - Takes the source para and transforms it using a single transformNode.
    para transform(transformNode node){
        int xInd=0;
        int yInd=0;
        int cosInd=0;
        int sinXInd=0;
        int sinYInd=0;
        expr samExp;
        para sPara;
        para final = copy();
        switch (node.type) {
        case TRANSLATE:
            final.x += node.val0;
            final.y += node.val1;
            break;
        case ROTATE:
            final = para(final.x*cos(node.val0 * pi / 180)-final.y*sin(node.val0 * pi / 180),final.y*cos(node.val0 * pi / 180)+final.x*sin(node.val0 * pi / 180));
            break;
        case SCALE:
            final.x *= node.val0;
            final.y *= node.val1;
            break;
        case P_TRANSLATE:
            final.x += node.val2;
            final.y += node.val3;
            break;
        case P_ROTATE:
            final = para(final.x*cos(node.val2 * pi / 180)-final.y*sin(node.val2 * pi / 180),final.y*cos(node.val2 * pi / 180)+final.x*sin(node.val2 * pi / 180));
            break;
        case P_SCALE:
            final.x *= node.val2;
            final.y *= node.val3;
            break;
        }
        final.optimize();
        return final;
    }

    // transform(multi) - Takes the source para and transforms it using a sequence of transformNodes.
    para transform(std::vector<transformNode> sequence){
        para final = copy();
        for (unsigned int i = 0; i < sequence.size(); i++)
            final = final.transform(sequence[i]);
        return final;
    }
};

/*
* angleVector - Returns the angle vector created from points p1 and p2.
*/
double angleVector(point p1, point p2) {
    return point(p2.x - p1.x, p2.y - p1.y).deriveAngle();
};

/*
* bez - Creates a bezier curve using control points co.
*/
para bez(std::vector<point> co) {
    std::vector<double> xPos;
    std::vector<double> yPos;
    for (unsigned int i = 0; i < co.size(); i++) {
        double sX = 0;
        double sY = 0;
        for (unsigned int j = 0; j < co.size() - i; j++) {
            sX += co[j].x * pascal(int(co.size()) - i - 1, j) * pow(-1, j + 1 + i);
            sY += co[j].y * pascal(int(co.size()) - i - 1, j) * pow(-1, j + 1 + i);
        }
        xPos.push_back(pascal(int(co.size()) - 1, i) * sX * pow(-1, int(co.size())));
        yPos.push_back(pascal(int(co.size()) - 1, i) * sY * pow(-1, int(co.size())));
    }
    return para(xPos, yPos);
};

/*
* sinusoid - Creates a sinsuoid(circle, arc, or spiral) based on the radius and degree values.
*/
para sinusoid(point center, double initRadius, double termRadius, double degOffset, double degAmt) {
    para final = para(
        (cos(poly(std::vector<double>({degAmt * pi / 180,degOffset * pi / 180})))*poly(std::vector<double>({termRadius - initRadius,initRadius})))+center.x,
        (sin(poly(std::vector<double>({degAmt * pi / 180,degOffset * pi / 180})))*poly(std::vector<double>({termRadius - initRadius,initRadius})))+center.y
    );
    final.optimize();
    return final;
}

/*
* compCurve - Takes the x component of para object x and y component of para object y and returns a new para object. Useful for sine waves.
*/
para compCurve(para x, para y) {
    return para(x.x, y.y);
}

/*
* transform - Takes a vector of para curves, transforms them based a sequence s, and returns the result.
*/
std::vector<para> transform(std::vector<para> l, std::vector<transformNode> s) {
    std::vector<para> final;
    for (unsigned int j = 0; j < l.size(); j++)
        final.push_back(l[j].transform(s));
    return final;
};

/*
* quickTransform - For use in para objects used as references in progressive wraps and warps.
*/
struct quickTransform {
    double scale{};
    double translation{};
    quickTransform() {}
    quickTransform(double s, double t) : scale(s), translation(t) {}
};

/*
* extend(expression) - Takes a para object and "extends" it based off a source expression. quickTransform t is used to instantly scale/translate the source.
*/
para extend(para c, expr a, quickTransform t){
    para dc = c.derive();
    para final = para(c.x+(a*dc.y)/sqrt((dc.x^2)+(dc.y^2)),c.y-(a*dc.x)/sqrt((dc.x^2)+(dc.y^2)));
    final.optimize();
    return final;
}

/*
* extend(constant) - Takes a para and "extends" it based of a constant value.
*/
para extend(para c, double a){
    para final = extend(c, expr(a), quickTransform(1,0));
    final.optimize();
    return final;
}

/*
* extend(start/end) - Takes a para and "extends" it based of a constant values s for start and e for end.
*/
para extend(para c, double s, double e){
    para final = extend(c, poly(std::vector<double>({e-s,s})), quickTransform(1,0));
    final.optimize();
    return final;
}

/*
* tangent - Takes sample curve s and maps it onto base b using points sp and bp.
*/
para tangent(para b, double bp, para s, double sp) {
    point stp = s(sp);
    para ds = s.derive();
    para db = b.derive();
    double sa = ds(sp).deriveAngle();
    double ba = db(bp).deriveAngle();
    point bap = b(bp);
    para final = s.transform(std::vector<transformNode>({transformNode(TRANSLATE, -stp.x, -stp.y),transformNode(ba - sa),transformNode(TRANSLATE, bap.x, bap.y)}));
    final.optimize();
    return final;
}

/*
* singleWarp(singular) - Takes sample curve l and maps it onto base b. points scope and offset are used for positioning.
*/
para singleWarp(para b, para l, point scope, point offset) {
    expr sx = (l.x+offset.x)/scope.x;
    expr sy = (l.y+offset.y)/scope.y;
    para db = b.derive();
    para final = para(comp(b.x, sx)+sy*comp(db.y, sx),
                      comp(b.y, sx)-sy*comp(db.y, sx)
                     );
    final.optimize();
    return final;
}

/*
* singleWarp(multi) - Takes sample curve vector l and maps all curves onto base b.
*/
std::vector<para> singleWarp(para b, std::vector<para> l, point scope, point offset) {
    std::vector<para> final;
    for(int i=0;i<l.size();i++)
        final.push_back(singleWarp(b,l[i],scope,offset));
    return final;
}

/*
* doubleWarp(singular) - Takes sample curve l and maps it onto bases b1 and b2.
*/
para doubleWarp(para b1, para b2, para l, point scope, point offset){
    expr sx = (l.x+offset.x)/scope.x;
    expr sy = (l.y+offset.y)/scope.y;
    expr compb1x=comp(b1.x, sx);
    expr compb1y=comp(b1.y, sx);
    para final = para((comp(b2.x, sx)-compb1x)*sy+compb1x,
                      (compb1y-comp(b2.y, sx))*sy+compb1y);
    final.optimize();
    return final;
}

/*
* doubleWarp(multi) - Takes sample curve vector l and maps all curves onto bases b1 and b2.
*/
std::vector<para> doubleWarp(para b1, para b2, std::vector<para> l, point scope, point offset){
    std::vector<para> final;
    for(int i=0;i<l.size();i++)
        final.push_back(doubleWarp(b1,b2,l[i],scope,offset));
    return final;
}

/*
* quadWarp(singluar) - Takes sample curve l and maps it onto the bases.
*/
para quadWarp(para sa, para na, para wa, para ea, para l, point scope, point offset){
    expr sx = (l.x+offset.x)/scope.x;
    expr sy = (l.y+offset.y)/scope.y;
    expr wcx = comp(wa.x, sy);
    expr wcy = comp(wa.y, sy);
    expr ecx = comp(ea.x, sy);
    expr ecy = comp(ea.y, sy);
    point swc = sa(0);
    point sec = sa(1);
    point nwc = na(0);
    point nec = na(1);
    expr scx = comp(sa.x-poly(std::vector<double>({sec.x - swc.x, swc.x})), sx);
    expr scy = comp(sa.y-poly(std::vector<double>({sec.y - swc.y, swc.y})), sx);
    expr ncx = comp(na.x-poly(std::vector<double>({nec.x - nwc.x, nwc.x})), sx);
    expr ncy = comp(na.y-poly(std::vector<double>({nec.y - nwc.y, nwc.y})), sx);
    expr fx1 = wcx+((ecx-wcx)*sx)+(scx*(expr(1)-sy))+ncx*sy;
    expr fy1 = wcy+((ecy-wcy)*sx)+(scy*(expr(1)-sy))+ncy*sy;

    expr wcx2 = comp(sa.x, sx);
    expr wcy2 = comp(sa.y, sx);
    expr ecx2 = comp(na.x, sx);
    expr ecy2 = comp(na.y, sx);
    point swc2 = wa(0);
    point sec2 = wa(1);
    point nwc2 = ea(0);
    point nec2 = ea(1);
    expr scx2 = comp(wa.x-poly(std::vector<double>({sec2.x - swc2.x, swc2.x})), sy);
    expr scy2 = comp(wa.y-poly(std::vector<double>({sec2.y - swc2.y, swc2.y})), sy);
    expr ncx2 = comp(ea.x-poly(std::vector<double>({nec2.x - nwc2.x, nwc2.x})), sy);
    expr ncy2 = comp(ea.y-poly(std::vector<double>({nec2.y - nwc2.y, nwc2.y})), sy);
    expr fx2 = wcx2+((ecx2-wcx2)*sy)+(scx2*(expr(1)-sx))+ncx2*sx;
    expr fy2 = wcy2+((ecy2-wcy2)*sy)+(scy2*(expr(1)-sx))+ncy2*sx;
    para final = para((fx1+fx2)/2,(fy1+fy2)/2);
    final.optimize();
    return final;
}

/*
* quadWarp(multi) - Takes sample curve vector l and maps all curves onto the bases.
*/
std::vector<para> quadWarp(para sa, para na, para wa, para ea, std::vector<para> l, point scope, point offset){
    std::vector<para> final;
    for(int i=0;i<l.size();i++)
        final.push_back(quadWarp(sa, na, wa, ea,l[i],scope,offset));
    return final;
}