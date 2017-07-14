#ifndef MATHVECTOR_H
#define MATHVECTOR_H

#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <stdexcept>

#include "common_vector.h"
#include "common_vector_initializer.h"
#include "functional.h"

namespace MathCore
{
    namespace AlgebraCore
    {
        template<typename T, typename R> class MathVectorOperatorApplier;

        template<typename T> class MathVector
        {
        private:
            static std::function<void (T&, const T&, const T&)> vectorSummator;
        
        public:
            template<typename U, typename R> friend class MathVectorOperatorApplier;

        public:
            MathVector(size_t size=1, T default_value=(T)0);
            MathVector(const std::vector<T>& values);
            MathVector(const std::map<size_t, T> mapped_values, size_t full_size);
            ~MathVector();


            size_t get_dimension();
            bool is_zeros();

            template<typename U> friend bool operator==(const MathVector<U>& left, const MathVector<U>& right);
            template<typename U> friend bool operator!=(const MathVector<U>& left, const MathVector<U>& right);

            template<typename U> friend MathVector<U> operator+(const MathVector<U>& left, const MathVector<U>& right);
            template<typename U> friend MathVector<U> operator-(const MathVector<U>& left, const MathVector<U>& right);
            template<typename U> friend U operator*(const MathVector<U>& left, const MathVector<U>& right);

            template<typename U> friend MathVector<U> operator*(const MathVector<U>& left, const U& right_value);
            template<typename U> friend MathVector<U> operator*(const U& left_value, const MathVector<U>& right);
            template<typename U> friend MathVector<U> operator/(const MathVector<U>& left, const U& right_value);
            template<typename U> friend MathVector<U> operator/(const U& left_value, const MathVector<U>& right);

            const MathVector& operator+=(const MathVector& other);
            const MathVector& operator-=(const MathVector& other);

            const MathVector& operator+=(const T& value);
            const MathVector& operator-=(const T& value);
            const MathVector& operator*=(const T& value);
            const MathVector& operator/=(const T& value);

        protected:
            CommonVector<T>* m_vectorData;
        };

        template<typename T> std::function<void (T&, const T&, const T&)> MathVector<T>::vectorSummator = 
                [](T& accumulator, const T& left_value, const T& right_value)
                {
                    accumulator += (left_value * right_value);
                };

        template<typename T> MathVector<T>::MathVector(size_t size, T default_value)
        {
            m_vectorData = CommonVectorInitializer::InitializeCommonVector<T>(size, default_value);
        }

        template<typename T> MathVector<T>::MathVector(const std::vector<T>& values)
        {
            m_vectorData = CommonVectorInitializer::InitializeCommonVector<T>(values);
        }

        template<typename T> MathVector<T>::MathVector(const std::map<size_t, T> mapped_values,
                                                       size_t full_size)
        {
            m_vectorData = CommonVectorInitializer::InitializeCommonVector<T>(mapped_values,
                                                                              full_size);
        }

        template<typename T> MathVector<T>::~MathVector()
        {
            delete m_vectorData;
        }

        template<typename T> size_t MathVector<T>::get_dimension()
        {
            return m_vectorData->get_size();
        }

        template<typename T> bool MathVector<T>::is_zeros()
        {
            return m_vectorData->is_null();
        }

        template<typename T> const MathVector<T>& MathVector<T>::operator+=(const MathVector& other)
        {
            if (other.get_dimension() == get_dimension())
            {
                CommonVector<T>* newValues = CommonVectorInitializer::InitializeCommonVector<T>(CommonVectorsSummator<T>(m_vectorData, other.vector_data));
                delete m_vectorData;
                m_vectorData = newValues;
                return *this;
            }
            else
            {
                throw std::length_error("Added vector has different dimenstion");
            }
        }

        template<typename T> const MathVector<T>& MathVector<T>::operator-=(const MathVector& other)
        {
            if (other.get_dimension() == get_dimension())
            {
                CommonVector<T>* newValues = CommonVectorInitializer::InitializeCommonVector<T>(CommonVectorsSubtractor<T>(m_vectorData, other.vector_data));
                delete m_vectorData;
                m_vectorData = newValues;
                return *this;
            }
            else
            {
                throw std::length_error("Subtracted vector has different dimenstion");
            }
        }

        template<typename T> const MathVector<T>& MathVector<T>::operator+=(const T& value)
        {
            CommonVector<T>* newValues = CommonVectorInitializer::InitializeCommonVector<T>(CommonVectorIncreaser<T>(m_vectorData, value));
            delete m_vectorData;
            m_vectorData = newValues;
            return *this;
        }

        template<typename T> const MathVector<T>& MathVector<T>::operator-=(const T& value)
        {
            CommonVector<T>* newValues = CommonVectorInitializer::InitializeCommonVector<T>(CommonVectorDecreaser<T>(m_vectorData, value));
            delete m_vectorData;
            m_vectorData = newValues;
            return *this;
        }

        template<typename T> const MathVector<T>& MathVector<T>::operator*=(const T& value)
        {
            CommonVector<T>* newValues = CommonVectorInitializer::InitializeCommonVector<T>(CommonVectorMultiplier<T>(m_vectorData, value));
            delete m_vectorData;
            m_vectorData = newValues;
            return *this;
        }

        template<typename T> const MathVector<T>& MathVector<T>::operator/=(const T& value)
        {
            CommonVector<T>* newValues = CommonVectorInitializer::InitializeCommonVector<T>(CommonVectorDivider<T>(m_vectorData, value));
            delete m_vectorData;
            m_vectorData = newValues;
            return *this;
        }

        template<typename T> bool operator==(const MathVector<T>& left, const MathVector<T>& right)
        {
            return CommonVectorsEqualityChecker<T>(left.m_vectorData, right.m_vectorData); 
        }

        template<typename T> bool operator!=(const MathVector<T>& left, const MathVector<T>& right)
        {
            return !CommonVectorsEqualityChecker<T>(left.m_vectorData, right.m_vectorData); 
        }

        template<typename T> MathVector<T> operator+(const MathVector<T>& left, const MathVector<T>& right)
        {
            if (left.get_dimension() == right.get_dimension())
            {
                std::vector<T> newValues = CommonVectorsSummator<T>(left.m_vectorData, right.m_vectorData);
                return MathVector<T>(newValues);
            }
            else
            {
                throw std::length_error("Summarizing vectors have different dimenstion");
            }
        }

        template<typename T> MathVector<T> operator-(const MathVector<T>& left, const MathVector<T>& right)
        {
            if (left.get_dimension() == right.get_dimension())
            {
                std::vector<T> newValues = CommonVectorsSubtractor<T>(left.m_vectorData, right.m_vectorData);
                return MathVector<T>(newValues);
            }
            else
            {
                throw std::length_error("Subtracting vectors have different dimenstion");
            }
        }

        template<typename T> T operator*(const MathVector<T>& left, const MathVector<T>& right)
        {
            if (left.get_dimension() == right.get_dimension())
            {
                T result = PairReducer<T,T>(MathVector<T>::vectorSummator, left.m_vectorData, right.m_vectorData);
                return result;
            }
            else
            {
                throw std::length_error("Multiplying vectors have different dimenstion");
            }
        }
        
        template<typename T> MathVector<T> operator*(const MathVector<T>& left, const T& right_value)
        {
            std::vector<T> newValues = CommonVectorMultiplier<T>(left.m_vectorData, right_value);
            return MathVector<T>(newValues);
        }

        template<typename T> MathVector<T> operator*(const T& left_value, const MathVector<T>& right)
        {
            std::vector<T> newValues = CommonVectorMultiplier<T>(right.m_vectorData, left_value);
            return MathVector<T>(newValues);
        }

        template<typename T> MathVector<T> operator/(const MathVector<T>& left, const T& right_value)
        {
            std::vector<T> newValues = CommonVectorDivider<T>(left.m_vectorData, right_value);
            return MathVector<T>(newValues);
        }

        template<typename T> MathVector<T> operator/(const T& left_value, const MathVector<T>& right)
        {
            std::vector<T> newValues = CommonVectorDivider<T>(right.m_vectorData, left_value);
            return MathVector<T>(newValues);
        }
    }
}


#endif//MATHVECTOR_H
