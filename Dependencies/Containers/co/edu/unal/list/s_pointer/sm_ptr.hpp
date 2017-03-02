#ifndef SM_PTR_HPP_INCLUDED
#define SM_PTR_HPP_INCLUDED
namespace ssp
{
	template <typename T> class sm_ptr
	{
		T* data;
	public:
		sm_ptr (T* value) : data (value) {}
		~sm_ptr ()
		{
			delete data;
		}
		T& operator* ()
		{
			return *data;
		}

		T* operator-> ()
		{
			return data;
		}

	};
}

#endif // SM_PTR_HPP_INCLUDED
