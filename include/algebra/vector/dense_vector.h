#ifndef DENSEVECTOR_H
#define DENSEVECTOR_H

#include <algorithm>
#include <vector>
#include <set>

#include "common_vector.h"

namespace MathCore
{
    namespace AlgebraCore
    {
        template<typename T> class DenseVector
            : public CommonVector<T>
        {
        public:
            typedef typename CommonVector<T>::fast_iterator base_fast_iterator;
            typedef typename std::map<size_t, T>::iterator mapped_iterator;
            typedef typename std::map<size_t, T>::const_iterator const_mapped_iterator;
            //--------iterators--------------
            class fast_iterator
                : public CommonVector<T>::fast_iterator
            {
            public:
                 friend DenseVector<T>;

                 typedef fast_iterator self_type;
                 typedef typename CommonVector<T>::fast_iterator* self_pointer;
                 typedef T value_type;

            protected:
                fast_iterator(const DenseVector& parent, size_t index=0)
                    : m_parent(parent)
                {
                   if (index == 0)
                   {
                        this->m_indexIterator = m_parent.m_notNullIndexes.begin();
                        this->m_index = *(this->m_indexIterator);
                   }
                   else if (index == this->m_parent.m_size)
                   {
                        this->m_indexIterator = this->m_parent.m_notNullIndexes.end();
                        this->m_index = this->m_parent.m_size;
                   }
                   else
                   {
                        if (this->m_parent.m_values.at(index) == 0)
                        {
                            for (this->m_indexIterator = this->m_parent.m_notNullIndexes.begin();
                                 this->m_indexIterator != this->m_parent.m_notNullIndexes.end();
                                 this->m_indexIterator++)
                            {
                                if (*(this->m_indexIterator) > index)
                                {
                                    this->m_index = *(this->m_indexIterator);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            this->m_index = index;
                            this->m_indexIterator
                                = std::find(this->m_parent.m_notNullIndexes.begin(),
                                            this->m_parent.m_notNullIndexes.end(),
                                            index);
                        }
                   }
                }

            public:
                self_pointer operator++()
                {
                    this->m_indexIterator++;
                    if (this->m_indexIterator == this->m_parent.m_notNullIndexes.end())
                    {
                        this->m_index = this->m_parent.m_size;
                    }
                    else
                    {
                        this->m_index = *this->m_indexIterator;
                    }
                    return this;
                }

                value_type operator*()
                {
                    return this->m_parent.m_values.at(*(this->m_indexIterator));
                }

                value_type value()
                {
                    return this->m_parent.m_values.at(*(this->m_indexIterator));
                }

            private:
                const DenseVector& m_parent;
                std::vector<size_t>::const_iterator m_indexIterator;
            };

        public:
            DenseVector(size_t size, T default_value=0)
                : CommonVector<T>(size, default_value)
            {
                this->m_values = std::vector<T>(size, default_value);
                if (default_value != 0)
                {
                    m_notNullIndexes = std::vector<size_t>(size, 0);
                    for (size_t index = 0; index < m_values.size(); ++index)
                    {
                        m_notNullIndexes.at(index) = index;
                    }
                }
            }

            DenseVector(const std::vector<T>& values)
                : CommonVector<T>(values),
                  m_values(values)
            {
               for (size_t index = 0; index < this->m_size; ++index)
               {
                   if (this->m_values.at(index) != 0)
                   {
                       this->m_notNullIndexes.push_back(index);
                   }
               }
            }

            DenseVector(const std::map<size_t, T>& mapped_values, size_t full_size)
                : CommonVector<T>(mapped_values, full_size)
            {
                m_values = std::vector<T>(full_size, (T)0);
                m_notNullIndexes = std::vector<size_t>(mapped_values.size(), 0);
                const_mapped_iterator notNullValue;
                size_t full_index = 0;
                for (notNullValue = mapped_values.begin();
                     notNullValue != mapped_values.end();
                     notNullValue++)
                {
                    m_values[notNullValue->first] = notNullValue->second;
                    m_notNullIndexes[full_index] = notNullValue->first;
                    full_index++;
                }
            }

            bool is_null()
            {
                return this->m_notNullIndexes.size() == 0;
            }

            //------------iteration-operators--------
            T get_value(size_t index)
            {
                return m_values.at(index);
            }

            T operator[](size_t index)
            {
                return get_value(index);
            }

            base_fast_iterator* fast_begin()
            {
                return new fast_iterator(*this, 0);
            }

            base_fast_iterator* fast_end()
            {
                return new fast_iterator(*this, this->m_size);
            }

        private:
            std::vector<T> m_values;
            std::vector<size_t> m_notNullIndexes;
        };
    }
}

#endif//DENSEVECTOR_H
