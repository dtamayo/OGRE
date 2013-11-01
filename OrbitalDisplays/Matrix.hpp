//          Copyright Robert Douglas 2012
// Distributed under the accompanying Software License, Version 1.0.
//    (See accompanying file license.txt or copy at
//          https://subversion.assembla.com/svn/rob_douglas_sandbox/trunk/license.txt)
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <memory>
#include <numeric>
#include <assert.h>

#include <iostream>

namespace Math
{
	namespace Detail 
	{
		template<unsigned i, unsigned j, unsigned k, unsigned LRows, unsigned SharedDimension, unsigned RColumns>
		struct Multiplier;

		template<unsigned i, unsigned j, unsigned LRows, unsigned SharedDimension, unsigned RColumns>
		struct Multiplier_ColReducer;
		
		template<unsigned i, unsigned LRows, unsigned SharedDimension, unsigned RColumns>
		struct Multiplier_RowReducer;
	}

	template<unsigned DefNRows, unsigned DefNColumns>
	class Matrix
	{
	public:
		const static unsigned NRows = DefNRows;
		const static unsigned NColumns = DefNColumns;
		
	public:
		Matrix();
		template<typename RangeT>
		Matrix(const RangeT& r);

		void printAll();

		double& operator[](int index);
		const double operator[](int index) const;

	private:
		template<unsigned LRows, unsigned SharedDimension, unsigned RColumns> 
		friend Matrix<LRows, RColumns> operator*( const Matrix<LRows, SharedDimension>& l,  const Matrix<SharedDimension, RColumns>& r);

		template<unsigned i, unsigned j, unsigned k, unsigned LRows, unsigned SharedDimension, unsigned RColumns>
		friend struct Detail::Multiplier;

		template<unsigned i, unsigned j, unsigned LRows, unsigned SharedDimension, unsigned RColumns>
		friend struct Detail::Multiplier_ColReducer;
		
		template<unsigned i, unsigned LRows, unsigned SharedDimension, unsigned RColumns>
		friend struct Detail::Multiplier_RowReducer;

	private:
		double _mat[NRows*NColumns];
	};

	Matrix<1, 1> quickRowVector(double val0);
	Matrix<1, 2> quickRowVector(double val0, double val1);
	Matrix<1, 3> quickRowVector(double val0, double val1, double val2);
	Matrix<1, 4> quickRowVector(double val0, double val1, double val2, double val3);
	Matrix<1, 5> quickRowVector(double val0, double val1, double val2, double val3, double val4);
	Matrix<1, 6> quickRowVector(double val0, double val1, double val2, double val3, double val4, double val5);

	Matrix<1, 1> quickColVector(double val0);
	Matrix<2, 1> quickColVector(double val0, double val1);
	Matrix<3, 1> quickColVector(double val0, double val1, double val2);
	Matrix<4, 1> quickColVector(double val0, double val1, double val2, double val3);
	Matrix<5, 1> quickColVector(double val0, double val1, double val2, double val3, double val4);
	Matrix<6, 1> quickColVector(double val0, double val1, double val2, double val3, double val4, double val5);


	// ===========================================================================
	//                        Implementation Details
	// ===========================================================================
	template<unsigned DefNRows, unsigned DefNColumns>
	inline Matrix<DefNRows, DefNColumns>::Matrix()
	{
		memset(_mat, 0, sizeof(double) * NRows * NColumns);
	}
	template<unsigned DefNRows, unsigned DefNColumns>
	template<typename RangeT>
	inline Matrix<DefNRows, DefNColumns>::Matrix(const RangeT& r)
	{
		assert(r.size() == DefNRows * DefNColumns);
		std::copy(r.begin(), r.end(), _mat);
	}
	template<unsigned DefNRows, unsigned DefNColumns>
	inline void Matrix<DefNRows, DefNColumns>::printAll()
	{
		for (unsigned i = 0; i < DefNRows; ++i)
		{
			for (unsigned j = 0; j < DefNColumns; ++j)
			{
				std::cout << _mat[i * DefNColumns + j] << " ";
			}
			std::cout << std::endl;
		}
	}
	template<unsigned DefNRows, unsigned DefNColumns>
	inline double& Matrix<DefNRows, DefNColumns>::operator[](int index)
	{
		return _mat[index];
	}
	template<unsigned DefNRows, unsigned DefNColumns>
	inline const double Matrix<DefNRows, DefNColumns>::operator[](int index) const
	{
		return _mat[index];
	}

	namespace Detail
	{
		template<
			unsigned i,
			unsigned j,
			unsigned k,
			unsigned LRows,
			unsigned SharedDimension,
			unsigned RColumns>
		struct Multiplier
		{
			static void multiply(
				double& output,
				const Matrix<LRows, SharedDimension>& l, 
				const Matrix<SharedDimension, RColumns>& r)
			{
				output += l._mat[i * SharedDimension + k] * r._mat[k * RColumns + j];
				Multiplier<i, j, k-1, LRows, SharedDimension, RColumns>::multiply(output, l, r);
			}
		};
		template<
			unsigned i,
			unsigned j,
			unsigned LRows,
			unsigned SharedDimension,
			unsigned RColumns>
		struct Multiplier<i, j, 0, LRows, SharedDimension, RColumns>
		{
			static void multiply(
				double& output,
				const Matrix<LRows, SharedDimension>& l, 
				const Matrix<SharedDimension, RColumns>& r)
			{
				output += l._mat[i * SharedDimension] * r._mat[j];
			}
		};
		template<
			unsigned i,
			unsigned j,
			unsigned LRows,
			unsigned SharedDimension,
			unsigned RColumns>
		struct Multiplier_ColReducer
		{
			static void multiply(
				Matrix<LRows, RColumns>& dest,
				const Matrix<LRows, SharedDimension>& l, 
				const Matrix<SharedDimension, RColumns>& r)
			{
				Multiplier<i, j, SharedDimension - 1, LRows, SharedDimension, RColumns>::multiply(dest._mat[i * RColumns + j], l, r);
				Multiplier_ColReducer<i, j - 1, LRows, SharedDimension, RColumns>::multiply(dest, l, r);
			}
		};
		template<
			unsigned i,
			unsigned LRows,
			unsigned SharedDimension,
			unsigned RColumns>
		struct Multiplier_ColReducer<i, 0, LRows, SharedDimension, RColumns>
		{
			static void multiply(
				Matrix<LRows, RColumns>& dest,
				const Matrix<LRows, SharedDimension>& l, 
				const Matrix<SharedDimension, RColumns>& r)
			{
				Multiplier<i, 0, SharedDimension - 1, LRows, SharedDimension, RColumns>::multiply(dest._mat[i * RColumns], l, r);
			}
		};

		template<
			unsigned i,
			unsigned LRows,
			unsigned SharedDimension,
			unsigned RColumns>
		struct Multiplier_RowReducer
		{
			static void multiply(
				Matrix<LRows, RColumns>& dest,
				const Matrix<LRows, SharedDimension>& l, 
				const Matrix<SharedDimension, RColumns>& r)
			{
				Multiplier_ColReducer<i, RColumns - 1, LRows, SharedDimension, RColumns>::multiply(dest, l, r);
				Multiplier_RowReducer<i - 1, LRows, SharedDimension, RColumns>::multiply(dest, l, r);
			}
		};
		template<
			unsigned LRows,
			unsigned SharedDimension,
			unsigned RColumns>
		struct Multiplier_RowReducer<0, LRows, SharedDimension, RColumns>
		{
			static void multiply(
				Matrix<LRows, RColumns>& dest,
				const Matrix<LRows, SharedDimension>& l, 
				const Matrix<SharedDimension, RColumns>& r)
			{
				Multiplier_ColReducer<0, RColumns - 1, LRows, SharedDimension, RColumns>::multiply(dest, l, r);
			}
		};
	} // namespace Detail
	template<unsigned LRows, unsigned SharedDimension, unsigned RColumns>
	inline
	Matrix<LRows, RColumns>
	operator*(
		const Matrix<LRows, SharedDimension>& l, 
		const Matrix<SharedDimension, RColumns>& r)
	{
		typedef Matrix<LRows, RColumns> ResultMatrix;
		ResultMatrix result;
		Detail::Multiplier_RowReducer<LRows - 1, LRows, SharedDimension, RColumns>::multiply(result, l, r);
		return result;
	}

	inline Matrix<1, 1> quickRowVector(double val0)
	{
		Matrix<1, 1> result;
		result[0] = val0;
		return result;
	}
	inline Matrix<1, 2> quickRowVector(double val0, double val1)
	{
		Matrix<1, 2> result;
		result[0] = val0;
		result[1] = val1;
		return result;
	}
	inline Matrix<1, 3> quickRowVector(double val0, double val1, double val2)
	{
		Matrix<1, 3> result;
		result[0] = val0;
		result[1] = val1;
		result[2] = val2;
		return result;
	}
	inline Matrix<1, 4> quickRowVector(double val0, double val1, double val2, double val3)
	{
		Matrix<1, 4> result;
		result[0] = val0;
		result[1] = val1;
		result[2] = val2;
		result[3] = val3;
		return result;
	}
	inline Matrix<1, 5> quickRowVector(double val0, double val1, double val2, double val3, double val4)
	{
		Matrix<1, 5> result;
		result[0] = val0;
		result[1] = val1;
		result[2] = val2;
		result[3] = val3;
		result[4] = val4;
		return result;
	}
	inline Matrix<1, 6> quickRowVector(double val0, double val1, double val2, double val3, double val4, double val5)
	{
		Matrix<1, 6> result;
		result[0] = val0;
		result[1] = val1;
		result[2] = val2;
		result[3] = val3;
		result[4] = val4;
		result[5] = val5;
		return result;
	}

	inline Matrix<1, 1> quickColVector(double val0)
	{
		Matrix<1, 1> result;
		result[0] = val0;
		return result;
	}
	inline Matrix<2, 1> quickColVector(double val0, double val1)
	{
		Matrix<2, 1> result;
		result[0] = val0;
		result[1] = val1;
		return result;
	}
	inline Matrix<3, 1> quickColVector(double val0, double val1, double val2)
	{
		Matrix<3, 1> result;
		result[0] = val0;
		result[1] = val1;
		result[2] = val2;
		return result;
	}
	inline Matrix<4, 1> quickColVector(double val0, double val1, double val2, double val3)
	{
		Matrix<4, 1> result;
		result[0] = val0;
		result[1] = val1;
		result[2] = val2;
		result[3] = val3;
		return result;
	}
	inline Matrix<5, 1> quickColVector(double val0, double val1, double val2, double val3, double val4)
	{
		Matrix<5, 1> result;
		result[0] = val0;
		result[1] = val1;
		result[2] = val2;
		result[3] = val3;
		result[4] = val4;
		return result;
	}
	inline Matrix<6, 1> quickColVector(double val0, double val1, double val2, double val3, double val4, double val5)
	{
		Matrix<6, 1> result;
		result[0] = val0;
		result[1] = val1;
		result[2] = val2;
		result[3] = val3;
		result[4] = val4;
		result[5] = val5;
		return result;
	}
} // Util

#endif

