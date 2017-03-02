#ifndef MAXPRIORITYQUEUE_HPP_INCLUDED
#define MAXPRIORITYQUEUE_HPP_INCLUDED
template <typename T> class MaxPriorityQueue
{
public:
	virtual bool isEmpty() = 0;
	virtual int getSize() = 0;
	virtual T getMax() = 0;
	virtual void put (T element) = 0;
	virtual T removeMax() = 0;
};


#endif // MAXPRIORITYQUEUE_HPP_INCLUDED
