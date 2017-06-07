#ifndef ORCS_PROPERTIES_H
#define ORCS_PROPERTIES_H

#include <map>
#include <string>
#include <set>
#include <sstream>


/**
 * This utility class is an alternative of a map to variable types. Internally 
 * it store data as string objects, then the data must have the stream operator 
 * implemented to store and retrieve the information.
 */
class Properties {

public:
    
    /**
     * Default constructor.
     */
    Properties() {
        // It does nothing.
    }
    
    /**
     * Copy constructor.
     * 
     * @param   other
     *          The object to be copied.
     */
    Properties(const Properties& other) : properties(other.properties) {
        // It does nothing.
    }
    
    /**
     * Destructor.
     */
    virtual ~Properties() {
        // It does nothing.
    }
    
    /**
     * Assignment operator. Makes this object a copy of the the specified 
     * object.
     * 
     * @param   other
     *          The object to be copied.
     * 
     * @return  A constant reference to this object.
     */
    virtual const Properties& operator=(const Properties& other) {
        this->properties.clear();
        this->properties.insert(other.properties.begin(), other.properties.end());
        return *this;
    }
    
    /**
     * The number of pairs key-value in this object.
     * 
     * @return  The number of pairs key-value in this object.
     */
    unsigned int size() const {
        return properties.size();
    }
    
    /**
     * Check if this object contains a property with the given key.
     * 
     * @param   key
     *          A key.
     * 
     * @return  True if this object contains a property of the given key, false 
     *          otherwise.
     */
    bool contains(std::string key) const {
        return (properties.count(key) > 0);
    }
    
    /**
     * Return a set with keys of the properties in this object.
     * 
     * @return  A set with keys of properties.
     */
    std::set<std::string> get_keys() const {
        std::set<std::string> keys;
        for (auto iter = properties.cbegin(); iter != properties.cend(); ++iter) {
            keys.insert(iter->first);
        }

        return keys;
    }
    
    /**
     * Get a property by its key.
     * 
     * @param   key
     *          The key.
     * 
     * @return  The value.
     */
    template<typename T>
    T get(const std::string& key) const {
        
        // Load the value into a stream
        std::stringstream stream;
        stream << properties.at(key);

        // Get the value
        T value;
        stream >> value;

        return value;
    }
    
    /**
     * Get a property by its key.
     * 
     * @param   key
     *          The key.
     * @param   default_value
     *          The value returned if this object does not contain a property 
     *          with the given key.
     * 
     * @return  The value.
     */
    template<typename T>
    T get(const std::string& key, T& default_value) const {

        // Check whether this object contains the given key
        if (contains(key)) {

            // Load the value into a stream
            std::stringstream stream;
            stream << properties.at(key);

            // Get the value
            T value;
            stream >> value;

            return value;
        }
        
        return default_value;
    }
    
    /**
     * Add a ney pair key-value.
     * 
     * @param   key
     *          The key.
     * @param   value
     *          The value.
     */
    template<typename T>
    void add(const std::string& key, const T& value) {

        // Load the value into a stream
        std::stringstream stream;
        stream << value;

        // Store the value
        properties[key] = stream.str();
    }

private:
    std::map<std::string, std::string> properties;

};


#endif
