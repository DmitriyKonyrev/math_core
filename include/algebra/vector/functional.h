#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <algorithm>
#include <functional>
#include <vector>

#include "common_vector.h"


namespace MathCore
{
    namespace AlgebraCore
    {
            template<typename T, typename R> R SingleReducer(std::function<void (R&, const T&)> reduce_operator,
                                                 const CommonVector<T>* data,
                                                 R initial_value = (R)0)
            {
                typename CommonVector<T>::fast_iterator* data_it = data->fast_begin();
                typename CommonVector<T>::fast_iterator* data_end = data->fast_end();

                R accumulator = initial_value;
                for (; (*data_it) != (*data_end); (*data_it)++)
                {
                    reduce_operator(accumulator, data_it->value());
                }

                return accumulator;
            } 

            template<typename T, typename R> R PairReducer(std::function<void (R&, const T&, const T&)> reduce_operator,
                                               const CommonVector<T>* left,
                                               const CommonVector<T>* right,
                                               R initial_value = (R)0)
            {
                typename CommonVector<T>::fast_iterator* left_it = left->fast_begin();
                typename CommonVector<T>::fast_iterator* left_end = left->fast_end();
                
                typename CommonVector<T>::fast_iterator* right_it = right->fast_begin();
                typename CommonVector<T>::fast_iterator* right_end = right->fast_end();

                R accumulator = initial_value;

                while ((*left_it) != (*left_end) || (*right_it) != (*right_end))
                {
                    if ((*left_it) == (*left_end))
                    {
                        reduce_operator(accumulator, T(0), right_it->value());
                        (*right_it)++;
                    }
                    else if ((*right_it) == (*right_end))
                    {
                        reduce_operator(accumulator, left_it->value(), (T)0);
                        (*left_it)++;
                    }
                    else
                    {
                        if ((*left_it) == (*right_it))
                        {
                            reduce_operator(accumulator, left_it->value(), right_it->value());
                            (*left_it)++;
                            (*right_it)++; 
                        }
                        else if ((*left_it) < (*right_it))
                        {
                            reduce_operator(accumulator, left_it->value(), (T)0);
                            (*left_it)++;
                        }
                        else
                        {
                            reduce_operator(accumulator, T(0), right_it->value());
                            (*right_it)++;
                        }
                    }
                }
                
                return accumulator;
            } 

            template<typename T, typename R> std::vector<R> SingleMapper(std::function<R (const T&)> map_operator,
                                                             const CommonVector<T>* data)
            {
                typename CommonVector<T>::fast_iterator* data_it = data->fast_begin();
                typename CommonVector<T>::fast_iterator* data_end = data->fast_end();

                std::vector<R> map_data(data->get_size(), (R)0);

                for (; (*data_it) != (*data_end); (*data_it)++)
                {
                    map_data.at(data_it->index()) = map_operator(data_it->value());
                }

                return map_data;
            } 

            template<typename T, typename R> std::vector<R> PairMapper(std::function<R (const T&, const T&)> map_operator,
                                                           const CommonVector<T>* left,
                                                           const CommonVector<T>* right)
            {
                typename CommonVector<T>::fast_iterator* left_it = left->fast_begin();
                typename CommonVector<T>::fast_iterator* left_end = left->fast_end();
                
                typename CommonVector<T>::fast_iterator* right_it = right->fast_begin();
                typename CommonVector<T>::fast_iterator* right_end = right->fast_end();
    
                size_t real_size = std::max(left->get_size(), right->get_size());
                std::vector<R> map_data(real_size, (R)0);

                while ((*left_it) != (*left_end) || (*right_it) != (*right_end))
                {
                    if ((*left_it) == (*left_end))
                    {
                        map_data.at(right_it->index()) = map_operator(T(0), right_it->value());
                        (*right_it)++;
                    }
                    else if ((*right_it) == (*right_end))
                    {
                        map_data.at(left_it->index()) = map_operator(left_it->value(), (T)0);
                        (*left_it)++;
                    }
                    else
                    {
                        if ((*left_it) == (*right_it))
                        {
                            map_data.at(left_it->index()) = map_operator(left_it->value(), right_it->value());
                            (*left_it)++;
                            (*right_it)++; 
                        }
                        else if ((*left_it) < (*right_it))
                        {
                            map_data.at(right_it->index()) = map_operator(T(0), right_it->value());
                            (*right_it)++;
                        }
                        else
                        {
                            map_data.at(left_it->index()) = map_operator(left_it->value(), (T)0);
                            (*left_it)++;
                        }
                    }
                }
                
                return map_data;
            }
           
            template<typename T> bool FastComparator(std::function<bool(const T&, const T&)> comparator,
                    const CommonVector<T>* left,
                    const CommonVector<T>* right)
            {
                typename CommonVector<T>::fast_iterator* left_it = left->fast_begin();
                typename CommonVector<T>::fast_iterator* left_end = left->fast_end();
                
                typename CommonVector<T>::fast_iterator* right_it = right->fast_begin();
                typename CommonVector<T>::fast_iterator* right_end = right->fast_end();
    
                bool comparing_result = true;

                while (comparing_result && ((*left_it) != (*left_end)) || ((*right_it) != (*right_end)))
                {
                    if ((*left_it) == (*left_end))
                    {
                        comparing_result &= comparator(T(0), right_it->value());
                        (*right_it)++;
                    }
                    else if ((*right_it) == (*right_end))
                    {
                        comparing_result &= comparator(left_it->value(), (T)0);
                        (*left_it)++;
                    }
                    else
                    {
                        if ((*left_it) == (*right_it))
                        {
                            comparing_result &= comparator(left_it->value(), right_it->value());
                            (*left_it)++;
                            (*right_it)++; 
                        }
                        else if ((*left_it) < (*right_it))
                        {
                            comparing_result &= comparator(T(0), right_it->value());
                            (*right_it)++;
                        }
                        else
                        {
                            comparing_result &= comparator(left_it->value(), (T)0);
                            (*left_it)++;
                        }
                    }
                }
                
                return comparing_result;
            }

            template<typename T> bool ValuesEquality(const T& left, const T& right)
            {
                return left == right;
            }

            template<typename T> bool CommonVectorsEqualityChecker(const CommonVector<T>* left, const CommonVector<T>* right)
            {
                return FastComparator<T>(ValuesEquality<T>, left, right); 
            }

            template<typename T> T PairSummator (const T& left, const T& right)
            { 
                return left + right; 
            };

            template<typename T> std::vector<T> CommonVectorsSummator(const CommonVector<T>* left, const CommonVector<T>* right)
            { 
                return PairMapper<T,T>(PairSummator<T>, left, right); 
            };

            template<typename T> T PairSubtractor (const T& left, const T& right)
            { 
                return left - right; 
            };

            template<typename T> std::vector<T> CommonVectorsSubtractor(const CommonVector<T>* left, const CommonVector<T>* right)
            { 
                return PairMapper<T,T>(PairSubtractor<T>, left, right); 
            };

            template<typename T> T ValueIncreaser (const T& vectorValue, const T& value)
            {
                return vectorValue + value;
            }

            template<typename T> T ValueDecreaser (const T& vectorValue, const T& value)
            {
                return vectorValue - value;
            }

            template<typename T> T ValueMultiplier (const T& vectorValue, const T& value)
            {
                return vectorValue * value;
            }

            template<typename T> T ValueDivider (const T& vectorValue, const T& value)
            {
                return vectorValue / value;
            }

            template<typename T> std::vector<T> CommonVectorIncreaser(const CommonVector<T>* data, const T& value)
            {
                return SingleMapper<T,T>([value](const T& v)->T { return ValueIncreaser<T>(v, value);}, data);
            }

            template<typename T> std::vector<T> CommonVectorDecreaser(const CommonVector<T>* data, const T& value)
            {
                return SingleMapper<T,T>([value](const T& v)->T { return ValueDecreaser<T>(v, value);}, data);
            }

            template<typename T> std::vector<T> CommonVectorMultiplier(const CommonVector<T>* data, const T& value)
            {
                return SingleMapper<T,T>([value](const T& v)->T { return ValueMultiplier<T>(v, value);}, data);
            }

            template<typename T> std::vector<T> CommonVectorDivider(const CommonVector<T>* data, const T& value)
            {
                return SingleMapper<T,T>([value](const T& v)->T { return ValueDivider<T>(v, value);}, data);
            }
            
            template<typename T> void MaxValue(T& accumulator, const T& value)
            {
                accumulator =  std::max(accumulator, value);
            }

            template<typename T> void MinValue(T& accumulator, const T& value)
            {
                accumulator =  std::min(accumulator, value);
            }

            template<typename T> void SumValue(T& accumulator, const T& value)
            {
                accumulator += value;
            }

            template<typename T> T Maximum(const CommonVector<T>* data)
            {
                return SingleReducer<T,T>(MaxValue<T>, data, data->fast_begin()->value());
            }

            template<typename T> T Minimum(const CommonVector<T>* data)
            {
                return SingleReducer<T,T>(MinValue<T>, data, data->fast_begin()->value());
            }   

            template<typename T> T ValuesSumm(const CommonVector<T>* data)
            {
                return SingleReducer<T,T>(SumValue<T>, data);
            }
    }
}

#endif//FUNCTIONAL_H
