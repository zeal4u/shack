#ifndef _SAFE_LOOKUP_TABLE_H_
#define _SAFE_LOOKUP_TABLE_H_

#include <functional>
#include <utility>
#include <list>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

namespace safe{

template<class Key, class Value, class Hash=std::hash<Key>>
class SafeLookupTable{
 private:
	class BucketType{
	 private:
		typedef std::pair<Key, Value> BucketValue;
		typedef std::list<BucketValue> BucketData;
		typedef typename BucketData::iterator BucketIterator;

		BucketData data_;
		mutable boost::shared_mutex mutex_;
		
		BucketIterator find_entry_for(Key const &key) const
		{
			return std::find_if(data_.begin(), data_.end(),
													[&](BucketValue const &item){
														return item.first == key;
													});
		}

	 public:
		Value value_for(Key const &key, Value const &default_value) const
		{
			// shared lock
			boost::shared_lock<boost::shared_mutex> lock(mutex_);
			BucketIterator const found_entry = find_entry_for(key);
			return (found_entry == data_.end()) ? 
							default_value : found_entry->second;
		}

		void add_or_update_mapping(Key const &key, Value const &value)
		{
			// mutex lock
			std::unique_lock<boost::shared_mutex> lock(mutex_);
			BucketIterator const found_entry = find_entry_for(key);
			if (found_entry == data_.end()) {
				data_.push_back(BucketValue(key, value));
			} else {
				found_entry->second = value;
			}
		}

		void remove_mapping(Key const &key)
		{
			// mutex lock
			std::unique_lock<boost::shared_mutex> lock(mutex_);	
			BucketIterator const found_entry = find_entry_for(key);
			if (found_entry != data_.end()) {
				data_.erase(found_entry);
			}
		}
	};	

	std::vector<std::unique_ptr<BucketType>> buckets_;
	Hash hasher_;

	BucketType& get_bucket(Key const &key) const
	{
		std::size_t const bucket_index = hasher(key) % buckets_.size();
		return *buckets_[bucket_index];
	}

 public:
	typedef Key KeyType;
	typedef Value MappedType;
	typedef Hash HashType;

	SafeLookupTable(unsigned num_buckets=19, Hash const &hasher=Hash()):
		buckets_(num_buckets), hasher_(hasher)
	{
		for (unsigned i = 0; i < num_buckets; ++i) {
			buckets_[i].reset(new BucketType);
		}
	}

	SafeLookupTable(const SafeLookupTable&) = delete;
	SafeLookupTable& operator=(const SafeLookupTable&) = delete;

	Value value_for(Key const &key, Value const &default_value=Value()) const
	{
		return get_bucket(key).value_for(key, default_value);
	}

	void add_or_update_mapping(Key const &key, Value const &value)
	{
		get_bucket(key).add_or_update_mapping(key, value);
	}

	void remove_mapping(Key const &key)
	{
		get_bucket(key).remove_mapping(key);
	}

	std::map<Key, Value> get_map() const
	{
		std::vector<std::unique_lock<boost::shared_mutex>> locks;
		for (unsigned i = 0; i < buckets_.size(); ++i) {
			locks.push_back(std::unique_lock<boost::shared_mutex>(buckets_[i].mutex_));
		}	
		std::map<Key, Value> res;
		for (unsigned i = 0; i < buckets_.size(); ++i) {
			for (auto it = buckets_[i].data_.begin();
				it != buckets_[i].data_.end();
				++it) {
				res.insert(*it);
			}
		}
		return res;
	}
};
} // namespace safe
#endif // _SAFE_LOOKUP_TABLE_H_
