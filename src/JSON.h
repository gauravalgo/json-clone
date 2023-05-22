#pragma once

//a helper macro to detect c++11 compliant compilers
#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include <initializer_list>

class JSON {
    private:
        static std::mutex _token;
    public:
        typedef enum {
            array,object,null,string,boolean,number_int,number_float
        } json_t;

    private:
        // the type of this object
        json_t _type;
        // the payload
        void* _payload;

    public:
        //a type for objects
        typedef std::tuple<std::string,JSON> object_t;
        // a type for arrays
        typedef std::initializer_list<JSON> array_t;
    public:
        //create a empty (null) object
        JSON();
        // create a string object from c++ string
        JSON(const std::string &);
        //create a string object from c string
        JSON(char *);
        //create a string object from c string
        JSON(const char *);
        //create a boolean object
        JSON(const bool);
        //create a number object
        JSON(const int);
        //create a number float object
        JSON(const double);
        //create from a intializer list (to an array)
        JSON(array_t);
        //create from a mapping (to an object)
        JSON(object_t);
        //copy constructor
        JSON(const JSON&);
        //move constructor
        JSON(JSON&&);

        //copy assignment 
        JSON& operator=(JSON);
        //destructor
        ~JSON();

        //implicit conversion to string representation
        operator const std::string() const;
        //implicit conversion to integer  (only for numbers)
        operator int() const;
        //implicit conversion to double  (only for numbers)
         operator double() const;
          //implicit conversion to Boolean  (only for numbers)
          operator bool() const;

          //write to stream 
          friend std::ostream& operator<<(std::ostream& o,const JSON & j){
            o << j.toString();
            return o;
          }
          //write to stream
          friend std::istream& operator<<(const JSON& j,std::ostream& o) {
            o << j.toString();
            return o;
          }
          
          //read from stream 
          friend std::istream& operator<<(JSON& j,std::istream& i){
            parser(i).parse(j);
            return i;
          }
          //read from stream
          friend std::istream& operator>>(std::istream& i,JSON &j){
            parser(i).parse(j);
            return i;
          }

          //explicit conversion to string representation (C++ style)
            const std::string toString() const;

        // add an object/array to an array
        JSON& operator+=(const JSON&);
        //add a string to an array
        JSON& operator+=(const std::string&);
        //add an char* to an array
        JSON& operator+=(const char*);
        //add a Boolean to an array
        JSON& operator+=(bool);
        //add an number to an array
        JSON operator+=(int);
        //add an number to an array
        JSON operator+=(double);

        //operator to set an element in an array
        JSON operator[](int);
        ///opertator to get an element in an array
        const JSON& operator[](int i) const;

        //operator to set an element in an object
        JSON& operator[](const std::string&);
        //operator to set an element in an object
        JSON& operator[](const char*);
        //operator to get an element in an object
        const JSON& operator[](const std::string&) const;

        //return the number of stored values
        size_t size() const;
        //check whether the object is empty
        bool empty() const;
        //return the type of object
        json_t type() const;

        //direct access to the underlying payload
        void* data();
        //direct access to the underlying payload
        const void* data() const;
        //lexicographic comparison of values
        bool operator==(const JSON&)const;
  
      private:
        //returns the type as string
        std::string _typename() const;

        //foeward declaration to the friend class
      public:
        class const_iterator;
      public:
        //an iterator to json
        class iterator {
          friend class JSON::const_iterator;
          public:
            iterator();
            iterator(JSON*);
            iterator(const iterator&);
            ~iterator();

            iterator& operator=(const iterator&);
            bool operator==(const iterator&);
            bool operator!=(const iterator&);
            
            iterator& operator++();
            JSON& operator*() const;
            JSON* operator->() const;

            //geetter for the key  (in case of objects)
            std::string key() const;
            //getter foe the values
            JSON& value() const;

          private:
            //a json value
            JSON* _object;
            //an iterator foe JSON arrays
            std::vector<JSON>::iterator* _vi;
            //an iterator for JSON objects
            std::map<std::string,JSON>::iterator* _oi;
          
        };

        //a const iterator
        class const_iterator {
        public:
          const_iterator();
          const_iterator(const JSON*);
          const_iterator(const const_iterator&);
          const_iterator(const iterator&);
          ~const_iterator();

          const_iterator& opertator=(const const_iterator&);
          bool operator==(const const_iterator&) const;
          bool operator!=(const const_iterator&) const;
          const_iterator& operator++();
          const JSON& operator*() const;
          const JSON& operator->() const;
          //getter for the key (in case of objects)
          std::string key() const;
          //getter for the value
          const JSON& value() const;
        private:
          //a JSON value
          const JSON* _object;
          //an iterator foe JSON arrays
          std::vector<JSON>::const_iterator* _vi;
          //an iterator for JSON objects
          std::map<std::string,JSON>::const_iterator* _oi;
        };
    public:
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const; 
    private:
        class parser{
          public:
            parser(char *);
            parser(std::string&);
            parser(std::istream&);

            ~parser();
            void parse(JSON&);
          private:
            bool next();
            void error(std::string = "");
            std::string parseString();
            void parseTrue();
            void parseFalse();
            void parseNull();
            void expect(char);
          
            char _current;
            char* _buffer;
            size_t _pos;
        };

};
