// *** ADDED BY HEADER FIXUP ***
#include <string>
// *** END ***
#ifndef LINEARLISTIMPROVED_HPP_INCLUDED
#define LINEARLISTIMPROVED_HPP_INCLUDED
namespace JList
{

	template <typename T>class LinearListImproved
	{
	public:
		virtual void save (std::string path) = 0;
		virtual void load (std::string path) = 0;
	};

}
#endif // LINEARLISTIMPROVED_HPP_INCLUDED
