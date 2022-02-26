#pragma once
#pragma warning( disable : 4201)
namespace types
{
	template <typename T = float> struct vec3d
	{
		static_assert(std::is_arithmetic_v<T>, "Value needs to be arithmetic");

		T x;
		T y;
		T z;
		vec3d() : x(0), y(0), z(0) { }
	};
	template <typename T = float> struct vec2d
	{
		static_assert(std::is_arithmetic_v<T>, "Value needs to be arithmetic");
		T x;
		T y;
		vec2d() : x(0), y(0) { }
	};
	template <typename T = float> struct vec4d
	{
		static_assert(std::is_arithmetic_v<T>, "Value needs to be arithmetic");
		T x;
		T y;
		T z;
		T w;
		vec4d() : x(0), y(0), z(0), w(0) { }
	};
	struct CRect : vec4d<int>
	{
		int& left() { return x; }
		int& right() { return z; }
		int& top() { return y; }
		int& bottom() { return w; }
	};
	using RECT = vec4d<int>;


	template <typename T> struct matrix 
	{
		static_assert(std::is_arithmetic_v<T>, "Arithmitix matrix");
		union
		{
			struct
			{
				T _11, _12, _13, _14;
				T _21, _22, _23, _24;
				T _31, _32, _33, _34;
				T _41, _42, _43, _44;
			};
			T m[4][4];
		};

		matrix() : _11(0), _12(0), _13(0), _14(0), _21(0), _22(0), _23(0), _24(0), _31(0), _32(0), _33(0), _34(0), _41(0), _42(0), _43(0), _44(0) { }
		explicit matrix(const T* pf)
		{
			if (pf)
				memcpy(&_11, pf, sizeof(matrix<T>));

		}
		matrix(const matrix<T>& mat)
		{
			memcpy(&_11, &mat, sizeof(matrix<T>));
		}
		matrix(T f11, T f12, T f13, T f14, T f21, T f22, T f23, T f24, T f31, T f32, T f33, T f34, T f41, T f42, T f43, T f44)
		{
			_11 = f11; _12 = f12; _13 = f13; _14 = f14;
			_21 = f21; _22 = f22; _23 = f23; _24 = f24;
			_31 = f31; _32 = f32; _33 = f33; _34 = f34;
			_41 = f41; _42 = f42; _43 = f43; _44 = f44;
		}

		T& operator()(unsigned int Row, unsigned int Col) { return m[Row][Col]; }
		T operator()(unsigned int Row, unsigned int Col) const { return m[Row][Col]; }

		explicit operator T*(){ return reinterpret_cast<T*>(&_11); }
		explicit operator const T*() const { return const_cast<const T*>(reinterpret_cast<T*>(&_11)); }

		matrix<T>& operator += (const matrix<T>& mat)
		{
			_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
			_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
			_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
			_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
			return *this;
		}
		matrix<T>& operator -= (const matrix<T>& mat)
		{
			_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
			_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
			_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
			_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
			return *this;
		}
		matrix<T>& operator *= (T f)
		{
			_11 *= f; _12 *= f; _13 *= f; _14 *= f;
			_21 *= f; _22 *= f; _23 *= f; _24 *= f;
			_31 *= f; _32 *= f; _33 *= f; _34 *= f;
			_41 *= f; _42 *= f; _43 *= f; _44 *= f;
			return *this;
		}
		matrix<T>& operator /= (T f)
		{
			if constexpr (std::is_floating_point_v<T>)
			{
				T fInv = 1.0f / f;
				_11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
				_21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
				_31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
				_41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
			}
			else
			{
				long double fInv = 1.0f / f;
				_11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
				_21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
				_31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
				_41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
				return *this;
			}
			return *this;
		}

		// unary operators
		matrix<T> operator + () const { return *this; }
		matrix<T> operator - () const { return matrix<T>(-_11, -_12, -_13, -_14, -_21, -_22, -_23, -_24, -_31, -_32, -_33, -_34, -_41, -_42, -_43, -_44); }
		matrix<T> operator + (const matrix<T>& mat) const 
		{ 
			return matrix<T>(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14, _21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
				_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34, _41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
		}
		matrix<T> operator - (const matrix<T>& mat) const
		{
			return matrix<T>(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14, _21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24, 
				_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34, _41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);

		}
		matrix<T> operator * (T f) const 
		{
			return matrix<T>(_11 * f, _12 * f, _13 * f, _14 * f, _21 * f, _22 * f, _23 * f, _24 * f, _31 * f, _32 * f, _33 * f, _34 * f, _41 * f, _42 * f, _43 * f, _44 * f);
		}
		matrix<T> operator/(T f) const 
		{
			if constexpr (std::is_floating_point_v<T>)
			{
				T fInv = 1.0f / f;
				return matrix<T>(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv, _21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
					_31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv, _41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);

			}
			else
			{
				long double fInv = 1.0f / f;
				return matrix<T>(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv, _21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
					_31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv, _41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);

			}
		}

		bool operator==(const matrix<T>& mat) const { return 0 == memcmp(this, &mat, sizeof(matrix<T>)); }
		bool operator!=(const matrix<T>& mat) const { return 0 != memcmp(this, &mat, sizeof(matrix<T>)); }
	};

	struct d3d9Mat
	{
		vec4d<float> Diffuse;
		vec4d<float> Ambient;
		vec4d<float> Specular;
		vec4d<float> Emissive;
		float Power{};
	};

}