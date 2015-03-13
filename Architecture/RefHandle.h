#ifndef _REF_HANDLE_H_
#define _REF_HANDLE_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include <stdexcept>

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Generic global Clone() functions
	//
	template< typename T >
	T* Clone( const T* data )
	{
		if( data )
			return data->Clone();
		else
			return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// [04/19/2011 kslee]
	// For those Ts without Clone() member function,
	// generic global Clone() function can not be instantiated.
	// In that case, template function specialization is needed.
	// such as, for primitive type bool,
	//
	// template<>
	// bool* Clone( const bool* data )
	// {
	//		if( data )
	//			return new bool( *data );
	//		else
	//			return 0;
	// }
	//

	//////////////////////////////////////////////////////////////////////////
	// Generic reference handle class
	//
	template< typename T >
	class RefHandle
	{
	public:
						RefHandle();
						RefHandle( const T& data );
						RefHandle( const T* data );
						RefHandle( const RefHandle<T>& refhData );
		virtual			~RefHandle();

						operator bool() const	{ return (m_data ? true : false); }
		RefHandle<T>&	operator=( const RefHandle<T>& rhs );			
		T&				operator*() const;
		T*				operator->() const;
		bool			operator==( const RefHandle<T>& rhs ) const;
		bool			operator<( const RefHandle<T>& rhs ) const;

		size_t			GetRefCnt() const		{ return *m_refPtr; }
		T*				GetDataPtr()			{ return m_data; }

	private:
		T*				m_data;
		size_t*			m_refPtr;
	};

	//////////////////////////////////////////////////////////////////////////
	// Implementation for RefHandle class
	//
	template< typename T >
	RefHandle<T>::RefHandle()
		: m_refPtr( new size_t( 1 ) ), m_data( 0 )
	{
	}

	template< typename T >
	RefHandle<T>::RefHandle( const T& data )
		: m_refPtr( new size_t( 1 ) ), m_data( 0 )
	{
		m_data = new T( data );
	}

	template< typename T >
	RefHandle<T>::RefHandle( const T* data )
		: m_refPtr( new size_t( 1 ) ), m_data( 0 )
	{
		m_data = Clone( data );
	}

	template< typename T >
	RefHandle<T>::RefHandle( const RefHandle<T>& refhData )
		: m_refPtr( refhData.m_refPtr ), m_data( refhData.m_data )
	{
		++(*m_refPtr);
	}

	template< typename T >
	RefHandle<T>::~RefHandle()
	{
		if( --(*m_refPtr) == 0 )
		{
			delete m_refPtr;
			delete m_data;
		}
	}

	template< typename T >
	RefHandle<T>& RefHandle<T>::operator=( const RefHandle<T>& rhs )
	{
		++(*rhs.m_refPtr);

		if( --(*m_refPtr) == 0 )
		{
			delete m_refPtr;
			delete m_data;
		}

		m_refPtr = rhs.m_refPtr;
		m_data = rhs.m_data;

		return *this;
	}

	template< typename T >
	T& RefHandle<T>::operator*() const
	{
		if( m_data )
		{
			return *m_data;
		}
		throw std::runtime_error( "unbound RefHandle" );
	}

	template< typename T >
	T* RefHandle<T>::operator->() const
	{
		if( m_data )
		{
			return m_data;
		}
		throw std::runtime_error( "unbound RefHandle" );
	}

	template< typename T >
	bool RefHandle<T>::operator==( const RefHandle<T>& rhs ) const
	{
		if( m_data && rhs.m_data )
			return (*m_data == *rhs.m_data);
		throw std::runtime_error( "unbound RefHandle" );
	}

	template< typename T >
	bool RefHandle<T>::operator<( const RefHandle<T>& rhs ) const
	{
		if( m_data && rhs.m_data )
			return (*m_data < *rhs.m_data);
		throw std::runtime_error( "unbound RefHandle" );
	}

	//////////////////////////////////////////////////////////////////////////
	// Implementation for RefHandle class
	//
	template< typename T >
	struct EqualForRefHandle
	{
		bool operator()( const T& x, const T& y) const	
		{
			if( x && y )
				return (*x == *y);
			throw std::runtime_error( "unbound RefHandle" );
		}
	};

	template< typename T >
	struct LessForRefHandle
	{
		bool operator()( const T& x, const T& y) const	
		{
			if( x && y )
				return (*x < *y);
			throw std::runtime_error( "unbound RefHandle" );
		}
	};
}

#endif