#ifndef COMMONVECTOR_H
#define COMMONVECTOR_H

#include <iterator>

namespace MathCore
{
    namespace AlgebraCore
    {
        /*! 
         * \class Abstract vector
         * \brief Describes the main functionality  of vector as values storage
         */
        template<typename T> class CommonVector
        {
        public:
            //-------------iterators---------------
            class iterator
            {
            public:
                typedef iterator self_type;
                typedef T value_type;
                typedef T& reference;
                typedef T* pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;   

            protected:
                iterator(size_t index=0) { m_index = index;  }
            
            public:
                bool operator==(const self_type& rhs) { return m_index == rhs.m_index; }
                bool operator!=(const self_type& rhs) { return m_index != rhs.m_index; }
                bool operator<(const self_type& rhs) { return m_index < rhs.m_index; }
                bool operator>(const self_type& rhs) { return m_index > rhs.m_index; }
                bool operator<=(const self_type& rhs) { return m_index <= rhs.m_index; }
                bool operator>=(const self_type& rhs) { return m_index >= rhs.m_index; }

            protected:
                size_t m_index;
            };

            class full_iterator
                : public iterator
            {
            public:
                typedef full_iterator self_type;
                typedef full_iterator* self_pointer;
                typedef T value_type;
                typedef T& reference;
                typedef T* pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;

            protected:
                full_iterator(const CommonVector *parent, size_t index=0) 
                    : iterator(index),
                      m_parent(parent) 
                { }

            public:
                self_pointer operator++() 
                { 
                    this->m_index++;        
                    return this;
                }

                value_type operator*()
                {
                    return m_parent->get_value(this->m_index);
                }

                value_type value()
                {
                    return m_parent->get_value(this->m_index);
                }

            private:
                const CommonVector* m_parent;
            };

            class fast_iterator
                : public iterator
            {
            public:
                typedef fast_iterator self_type;
                typedef fast_iterator* self_pointer;
                typedef T value_type;
                typedef T& reference;
                typedef T* pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;

            protected:
                fast_iterator(size_t index=0) : iterator(index) { }

            public:
                virtual self_pointer operator++() { return this; }
                virtual value_type value() { return (T)0; };
                virtual size_t index() { return this->m_index; }
            };

        public:
            //-------------constructors-------------
            CommonVector(size_t size, T default_value=0)
                : m_size(size)
            { };

            CommonVector(const std::vector<T>& values)
                : m_size(values.size())
            { }

            CommonVector(const std::map<size_t, T>& mapped_values, size_t full_size)
                : m_size(full_size)
            { }

            //------------usual-operators-----------
            size_t get_size() { return m_size; };
            virtual bool is_null() = 0;

            //------------iteration-operators--------
            virtual T get_value(size_t index) = 0;
            virtual T operator[](size_t index) = 0;

            virtual fast_iterator* fast_begin() = 0;
            virtual fast_iterator* fast_end() = 0;

            full_iterator* begin() { return new full_iterator(this, 0); }
            full_iterator* end() { return new full_iterator(this, m_size); }

        protected:
            size_t m_size;
            float m_load_factor;
        };
    }
}

#endif//COMMONVECTOR_H
