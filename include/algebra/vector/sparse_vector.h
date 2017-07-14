#ifndef SPARSEVECTOR_H
#define SPARSEVECTOR_H

#include <algorithm>
#include <map>
#include <vector>

#include "common_vector.h"

namespace MathCore
{
    namespace AlgebraCore
    {
        template<typename T> class SparseVector
            : public CommonVector<T>
        {
        public:
            typedef typename CommonVector<T>::fast_iterator base_fast_iterator;
            typedef typename std::map<size_t, T>::const_iterator mapped_iterator;

            //--------iterators---------------
            class fast_iterator
                : public CommonVector<T>::fast_iterator
            {
            public:
                friend SparseVector<T>;

                typedef fast_iterator self_type;
                typedef typename CommonVector<T>::fast_iterator* self_pointer;
                typedef T value_type;

            protected:
                fast_iterator(const SparseVector& parent, size_t index=0)
                    : m_parent(parent)
                {
                    if (index == 0)
                    {
                        this->m_valueIterator = this->m_parent.m_notNullValues.begin();
                        this->m_index = this->m_valueIterator->first;
                    }
                    else if (index == this->m_parent.m_size)
                    {
                        this->m_valueIterator = this->m_parent.m_notNullValues.end();
                        this->m_index = this->m_parent.m_size;

                    }
                    else
                    {
                        mapped_iterator  value_it = this->m_parent.m_notNullValues.find(index);
                        if (value_it != this->m_parent.m_notNullValues.end())
                        {
                            this->m_valueIterator = value_it;
                        }
                        else
                        {
                            mapped_iterator nearestNotNull;
                            for (nearestNotNull = this->m_parent.m_notNullValues.begin();
                                 nearestNotNull != this->m_parent.m_notNullValues.end();
                                 nearestNotNull++)
                            {
                                if (index >= nearestNotNull->first)
                                    break;
                            }
                        }
                        this->m_index = this->m_valueIterator->first;
                    }
                }

            public:
                self_pointer operator++()
                {
                    this->m_valueIterator++;
                    if (this->m_valueIterator != this->m_parent.m_notNullValues.end())
                    {
                        this->m_index = this->m_valueIterator->first;
                    }
                    else
                    {
                        this->m_index = this->m_parent.m_size;
                    }

                    return this;
                }

                value_type operator*()
                {
                    return this->m_valueIterator->second;
                }

                value_type value()
                {
                    return this->m_valueIterator->second;
                }

            private:
                const SparseVector& m_parent;
                mapped_iterator m_valueIterator;
            };

        public:
            SparseVector(size_t size, T default_value=0.0)
                : CommonVector<T>(size, default_value)
            {
                if (default_value == 0.0)
                {
                    for (size_t index = 0; index < size; ++index)
                    {
                        this->m_notNullIndexes.insert(index);
                        this->m_notNullValues.insert(std::make_pair(index, default_value));
                    }
                }
            }

            SparseVector(const std::vector<T>& values)
                : CommonVector<T>(values)
            {
                for (size_t index = 0; index < this->m_size; ++index)
                {
                    if (values.at(index) != 0)
                    {
                        this->m_notNullIndexes.insert(index);
                        this->m_notNullValues.insert(std::make_pair(index, values.at(index)));
                    }
                }
            }

            SparseVector(const std::map<size_t, T> mapped_values, size_t full_size)
                : CommonVector<T>(mapped_values, full_size),
                  m_notNullValues(mapped_values)
            {
                for (const auto& key_value: m_notNullValues)
                    m_notNullIndexes.insert(key_value.first);
            };

            //------------iteration-operators--------
            T get_value(size_t index)
            {
                mapped_iterator value_it = this->m_notNullValues.find(index);
                if (value_it != this->m_notNullValues.end())
                {
                    return value_it->second;
                }
                else
                {
                    return (T)0;
                }
            }

            bool is_null()
            {
                return this->m_notNullIndexes.size() == 0;
            }

            T operator[](size_t index)
            {
               return this->get_value(index);
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
            std::map<size_t, T> m_notNullValues;
            std::set<size_t> m_notNullIndexes;
        };
    }
}

#endif//SPARSEVECTOR_H
