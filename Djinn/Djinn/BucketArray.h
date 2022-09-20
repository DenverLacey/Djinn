#pragma once

#include <forward_list>
#include <assert.h>

#include "skrive.h"

template<typename T>
class BucketArray
{
private:
	using Bucket = T*;

public:
	BucketArray() = default;
	BucketArray(size_t bucket_size) : bucket_size(bucket_size), length(0) {}
	BucketArray(const BucketArray<T>& other) = default;
	BucketArray(BucketArray<T>&& other) = default;

	BucketArray<T>& operator=(const BucketArray<T>& other) = default;
	BucketArray<T>& operator=(BucketArray<T>&& other) = default;

public:
	size_t size() const
	{
		return length;
	}

	T& top() const
	{
		assert(length > 0);
		size_t write_index = length % bucket_size;
		auto bucket = buckets.front();
		return bucket[write_index];
	}

public:
	void append(const T& item)
	{
		size_t write_index = length % bucket_size;

		if (write_index >= bucket_size || buckets.empty())
		{
			allocate_bucket();
			write_index = 0;
		}

		auto bucket = buckets.front();
		bucket[write_index] = item;

		length++;
	}

private:
	void allocate_bucket()
	{
		Bucket new_bucket = new T[bucket_size];
		buckets.push_front(new_bucket);
	}
		
private:
	size_t bucket_size;
	size_t length;
	std::forward_list<Bucket> buckets;

	friend struct sk::Formatter<BucketArray<T>>;
};

template<typename T>
struct sk::Formatter<BucketArray<T>>
{
	static void format(const BucketArray<T>& arr, std::string_view fmt, Writer& writer)
	{
		writer.write("{ ");

		size_t i = 0;
		for (T* bucket : arr.buckets)
		{
			for (size_t bi = 0; i < arr.bucket_size && i < arr.size(); bi++, i++)
			{
				T& item = bucket[bi];
				writer.print("{} ", item);
			}
		}

		writer.write('}');
	}
};
