#ifndef MATHVECTORNORM_H
#define MATHVECTORNORM_H

#include <vector>
#include <cmath>

#include "common_vector.h"
#include "functional.h"
#include "math_vector.h"
#include "math_vector_applier.h"

namespace MathCore
{
    namespace AlgebraCore
    {
        template<typename T> class MathVectorNorm
            : public MathVectorOperatorApplier<T, float>
        {
        public:
            MathVectorNorm() {};
            
            virtual float operator()(const MathVector<T>& vector) = 0;
        };

        template<typename T> class MVectorNorm
            : public MathVectorNorm<T>
        {
        public:
            MVectorNorm() : MathVectorNorm<T>() {}
            
            float operator()(const MathVector<T>& vector)
            {
               return (float)applySingleReducer(Maximum<T>,vector);
            }
        };

        template<typename T> class L1VectorNorm
            : public MathVectorNorm<T>
        {
        private:
            static void AbsoluteSum(float& acc, const T& value)
            {
                acc += abs((float)value);
            }

        public:
            L1VectorNorm() : MathVectorNorm<T>() {}
            
            float operator()(const MathVector<T>& vector)
            {
               return applySingleReduceOperator(AbsoluteSum, vector);
            }
        };

        template<typename T> class HolderVectorNorm
            : public MathVectorNorm<T>
        {
        private:
            static void DegreeSum(float& acc, const T& value, const float& degree)
            {
                acc += pow((float)value, degree);
            }

        public:
            HolderVectorNorm(float degree = 2.0) 
                : MathVectorNorm<T>(),
                  m_degree(degree),
                  m_inverseDegree(1. / degree)
                  
            { }
            
            float operator()(const MathVector<T>& vector)
            {
               return pow(applySingleReduceOperator(
                          [this](float& acc, const T& value) 
                          { DegreeSum(acc, value, m_degree); }
                          , vector)
                        , m_inverseDegree);
            }

            static HolderVectorNorm Euclidean()
            {
                return HolderVectorNorm(2.0);
            }

        private:
            float m_degree; 
            float m_inverseDegree;
        };
    }
}

#endif//MATHVECTORNORM_H
