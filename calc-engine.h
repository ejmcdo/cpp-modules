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
};

// pow - Raises a poly object x to the nth power and returns the result.
poly pow(poly x, int n){
    poly final=x;
    for(int i=1;i<n;i++)
        final *= x;
    return final;
}

/*
* dPoly - Returns the derivative the polynomial x.
*/
poly dPoly(poly x) {
    std::vector<double> final;
    for (int i = 0; i < int(x.c.size()) - 1; i++) // Derivative of a polynomial has a degree thats 1 less than its original.
        final.push_back(x.c[i] * (x.c.size() - 1 - i)); // The value multiplied by the power is applied to the vector.
    return poly(final);
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
std::vector<std::string> expNodeNames = std::vector<std::string>({ "polynomial","compound","fraction","constant" });

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
std::vector<std::string> expNodeFuncNames = std::vector<std::string>({ "none","square root","sine","cosine" });

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
            std::cout << val2[0] << " " << val2[1]; // FRACTION print their numerator and denominator values.
            break;
        case CONSTANT:
            std::cout << val3;
            break;
        }
        if (type != CONSTANT)
            std::cout << " [" << expNodeFuncNames[int(f)] << ", " << (n ? "negated" : "!negated") << "]"; // For all types except CONSTANT, function type and negation are printed.
    }
    bool operator==(expNode x){
        bool final;
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

    expr() {}
    expr(std::vector<expNode> li) : l(li) {}

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
                           // Example: If an expNode has an index value of -1 and is pushed to an exp object whose node list is of length 10, then that index of the expNode becomes -1+10, or 9, referring to the then last value of the expNode in the list
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
                final.push(expNode(dPoly(final.l[i].val0), NONE_FUNC, false));
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
            case SQRT: // For SQRT functions, the derivative is evaluated as A/(2*sqrt(a)).
                final.push(expNode(2));
                final.push(expNode(std::vector<std::vector<int>>({ std::vector<int>({int(i),-1}) }), NONE_FUNC, false));
                final.push(expNode(std::vector<int>({ -3,-1 }), NONE_FUNC, false));
                break;
            case SIN: // For SIN functions, the derivative is evaluated as Acos(a)
                sNode = final.l[i];
                sNode.f = COS;
                final.push(sNode);
                final.push(expNode(std::vector<std::vector<int>>({ std::vector<int>({-2,-1}) }), NONE_FUNC, false));
                break;
            case COS: // For COS functions, the derivative is evaluated as -Asin(a)
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

    // deriveAngle - Calculates the angle made by the vector between the point and the origin in standard notation. Returns in radians by default, but will return in degrees if deg is set to true.
    double deriveAngle(bool deg) {
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
        if(deg)
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
* dist - Represents a point slope function in either the form y = mx + b or x = my + b.
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
