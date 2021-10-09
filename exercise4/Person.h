#ifndef PERSON_H
#define PERSON_H
/* 
 * File:   Person.h
 * 
 * Copyright (C) 2016 raodm@miamiOH.edu
 */

#include <iostream>
#include <string>

/**
 * A simple class that encapsulates id, age, and name.
 */
class Person {
    friend std::ostream& operator<<(std::ostream& os, const Person& p);
    friend std::istream& operator>>(std::istream& is, Person& p);
    // friend std::istream& operator<(const Person& p1, const Person& p2);


   public:
    bool operator<(const Person& other) const;
    /**
     * Constructor that serves as default (no-argument) and
     * a convenience constructor.
     * 
     * @param id The ID for the person
     * @param age The age for the person.
     * @param name The name of the person.
     */
    Person(int id = -1, unsigned short age = -1, 
           const std::string& name = "");
    
    /** Destructor.  
     * 
     * Simply a place holder
     */
    virtual ~Person();
    
    /**
     * Reads a Person information from a given input stream.
     * 
     * This 1 line method reads data in the order id (int), age (short), 
     * name (string in double quotes using std::quoted).
     * 
     * @param is The input stream from where the data is to be read
     * 
     * @return Returns the input stream as a convenience to permit
     * chaining calls.
     */
    // std::istream& operator read(std::istream& is = std::cin);

    /**
     * Writes a Person information from a given output stream.
     * 
     * This 1 line method writes data in the order id (int), age (short), 
     * and quoted name using 1 space (" ") as separator between them.
     * 
     * @param is The input stream from where the data is to be read
     * 
     * @return Returns the input stream as a convenience to permit
     * chaining calls.
     */    
    // std::ostream& print(std::ostream& os = std::cout) const;
    
    /**
     * A comparison operator that compares the age of employees.
     * 
     * This 1 line method returns true if age of 'this' employee
     * is lower than the age of 'other' employee.
     * 
     * @param other The other person to be used for comparison.
     * 
     * @return This 1 line method returns true if age of 'this' employee
     * is lower than the age of 'other' employee.
     */
    // bool lessThan(const Person& other) const;
    
private:
    /// The id of the user.
    int id;
    /// The age of the user (obviously cannot be negative)
    unsigned short age;
    /// The name of the user.
    std::string name;
};

// std::istream& operator > (std::istream& is,Person& p)
#endif /* PERSON_H */
