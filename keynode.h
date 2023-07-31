#ifndef __KEYNODE_H__
#define __KEYNODE_H__

template <typename T, typename V>
class KeyNode
{
public:
  using value_type   = T;
  using Type      = T;
  using LinkedValueType = V;
private:
  using Node      = KeyNode<T, V> ;
public:
    value_type       m_key;
    LinkedValueType      m_value;

public:
    KeyNode(value_type key, LinkedValueType value) 
        : m_key(key), m_value(value) {}

    KeyNode(const KeyNode<T, V>& other) : 
        KeyNode(other.m_key, other.m_value) {}

    KeyNode(KeyNode<T, V>&& other) // Move constructor
        : m_key  (std::move(other.m_key)), 
          m_value(std::move(other.m_value)) {}
    KeyNode() {}

    KeyNode& operator=(const KeyNode& other) {
        if (this != &other) {
            m_key = other.m_key;
            m_value = other.m_value;
        }
        return *this;
    }

    value_type    getData() const   { return m_key; }
    value_type&   getDataRef()      { return m_key; }
    LinkedValueType  getValue() const { return m_value; }
    LinkedValueType& getValueRef()    { return m_value; }

    bool operator<(const KeyNode<T, V>& other) const { 
        return m_key < other.m_key;
    }
    // Error was here. Next line was missing
    bool operator>(const KeyNode<T, V>& other) const { 
        return m_key > other.m_key;
    }

    constexpr operator value_type() const noexcept { // since C++14
        return m_key;
    }

};

#endif