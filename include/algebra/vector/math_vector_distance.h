#ifndef MATHVECTORDISTANCE_H
#define MATHVECTORDISTANCE_H

#include <vector>
#include <cmath>
#include <limits>

#include "common_vector.h"
#include "functional.h"
#include "math_vector.h"
#include "math_vector_applier.h"

namespace MathCore
{
    namespace AlgebraCore
    {
        template<typename T> class MathVectorDistance
            : public MathVectorOperatorApplier<T, float> 
        {
        public:
            MathVectorDistance() {};

            virtual float operator()(const MathVector<T>& left, const MathVector<T>& right) = 0;
        };

        template<typename T> class ManhattanDistance
            : public MathVectorDistance<T>
        {
        private:
            static void SumAbsolute(float& acc, const T& left_value, const T& right_value)
            {
                acc += abs((float)left_value - (float)right_value);
            }

        public:
            ManhattanDistance() {};

            float operator()(const MathVector<T>& left, const MathVector<T>& right)
            {
                return applyPairReduceOperator(SumAbsolute, left, right);
            }
        };

        template<typename T> class HolderDistance
            : public MathVectorDistance<T>
        {
        private:
            static void sumDegree(float& acc, const T& left_value, const T& right_value, float degree)
            {
                acc += pow((float)left_value - (float)right_value, degree);
            }

        public:
            HolderDistance(float degree = 2.0)
                : MathVectorDistance<T>()
                  , m_degree(degree)
            {
                m_inverseDegree = 1. / m_degree;
            };

            float operator()(const MathVector<T>& left, const MathVector<T>& right)
            {
                return pow(applyPairReduceOperator(
                            [this](float& acc, const T& l_val, const T& r_val)
                            { sumDegree(acc, l_val, r_val, m_degree); }
                            , left, right, 0.0)
                        , m_inverseDegree);
            }

        private:
            float m_degree;
            float m_inverseDegree;
        };

        template<typename T> class MathVectorSimiliarity
            : public MathVectorDistance<T>
        {
        protected:
            static void sumMinimum(float& acc, const T& left_value, const T& right_value)
            {
                acc += min((float)left_value, (float)right_value);
            }

            static void sumMaximum(float& acc, const T& left_value, const T& right_value)
            {
                acc += max((float)left_value, (float)right_value);
            }

            virtual float softIntersection(const MathVector<T>& left, const MathVector<T>& right)
            {
                return 0.0;
            };

            virtual float softUnion(const MathVector<T>& left, const MathVector<T>& right)
            {
                return 0.0;
            }

        public:
            MathVectorSimiliarity() : MathVectorDistance<T>() {}

            float operator()(const MathVector<T>& left, const MathVector<T>& right)
            {
                float similiarity = 1.0;
                if (!left.is_zeros() || !right.is_zeros())
                {
                    float soft_intersection = softIntersection(left, right);
                    float soft_union = softUnion(left, right);
                    if (soft_union == 0.0)
                    {
                        similiarity = -std::numeric_limits<double>::infinity();
                    }
                    else
                    {
                        similiarity = soft_intersection / soft_union;
                    }
                }

                return similiarity;
            }
        };

        template<typename T> class JaccardSimiliarity
            : public MathVectorSimiliarity<T>
        {
        private:
            float softIntersection(const MathVector<T>& left, const MathVector<T>& right)
            {
               return applyPairReduceOperator(MathVectorSimiliarity<T>::sumMinimum, left, right);
            }

            float softUnion(const MathVector<T>& left, const MathVector<T>& right)
            {
                return applyPairReduceOperator(MathVectorSimiliarity<T>::sumMaximum, left, right);
            }

        public:
            JaccardSimiliarity() : MathVectorSimiliarity<T>() {};
        };

        template<typename T> class SorensenSimiliarity
            : public MathVectorSimiliarity<T>
        {
        private:
            float softIntersection(const MathVector<T>& left, const MathVector<T>& right)
            {
               return 2.0 * applyPairReduceOperator(MathVectorSimiliarity<T>::sumMinimum, left, right);
            }

            float softUnion(const MathVector<T>& left, const MathVector<T>& right)
            {
                return (float)(applySingleReducer(ValuesSumm<T>, left) + applySingleReducer(ValuesSumm<T>, right));
            }

        public:
            SorensenSimiliarity() : MathVectorSimiliarity<T>() {};
        };

        template<typename T> class SimpsonSimiliarity
            : public MathVectorSimiliarity<T>
        {
        private:
            float softIntersection(const MathVector<T>& left, const MathVector<T>& right)
            {
               return applyPairReduceOperator(MathVectorSimiliarity<T>::sumMinimum, left, right);
            }

            float softUnion(const MathVector<T>& left, const MathVector<T>& right)
            {
                return min((float)applySingleReducer(ValuesSumm<T>, left), applySingleReducer(ValuesSumm<T>, right));
            }

        public:
            SimpsonSimiliarity() : MathVectorSimiliarity<T>() {};
        };

        template<typename T> class KulchinskiySimiliarity
            : public MathVectorSimiliarity<T>
        {
        private:
            float softIntersection(const MathVector<T>& left, const MathVector<T>& right)
            {
               return 0.5 * applyPairReduceOperator(MathVectorSimiliarity<T>::sumMinimum, left, right);
            }

            float softUnion(const MathVector<T>& left, const MathVector<T>& right)
            {
                float leftAbundance = applySingleReducer(ValuesSumm<T>, left);
                float rightAbundance = applySingleReducer(ValuesSumm<T>, right);
                float intersection = 0.0;
                if (leftAbundance != 0.0 && rightAbundance != 0.0)
                {
                    intersection = 1.0 / (leftAbundance + rightAbundance);
                }
                return intersection;
            }

        public:
            KulchinskiySimiliarity() : MathVectorSimiliarity<T>() {};
        };

        template<typename T> class OtiaiSimiliarity
            : public MathVectorSimiliarity<T>
        {
        private:
            float softIntersection(const MathVector<T>& left, const MathVector<T>& right)
            {
               return applyPairReduceOperator(MathVectorSimiliarity<T>::sumMinimum, left, right);
            }

            float softUnion(const MathVector<T>& left, const MathVector<T>& right)
            {
                return sqrt(abs(applySingleReducer(ValuesSumm<T>, left) * applySingleReducer(ValuesSumm<T>, right)));
            }

        public:
            OtiaiSimiliarity() : MathVectorSimiliarity<T>() {};
        };

        template<typename T> class BrownBlankeSimiliarity
            : public MathVectorSimiliarity<T>
        {
        private:
            float softIntersection(const MathVector<T>& left, const MathVector<T>& right)
            {
               return applyPairReduceOperator(MathVectorSimiliarity<T>::sumMinimum, left, right);
            }

            float softUnion(const MathVector<T>& left, const MathVector<T>& right)
            {
                return max(applySingleReducer(ValuesSumm<T>, left), applySingleReducer(ValuesSumm<T>, right));
            }

        public:
            BrownBlankeSimiliarity() : MathVectorSimiliarity<T>() {};
        };
    }
}

#endif//MATHVECTORDISTANCE_H
