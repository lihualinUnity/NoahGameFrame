﻿//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#ifndef NFREDISPLUGIN_NFREDISCLIENT_H
#define NFREDISPLUGIN_NFREDISCLIENT_H


#include <string>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <ctime>
#include <sstream>
#include <iostream>
#include <random>

#include "NFRedisResult.h"
#include "NFRedisCommand.h"

typedef std::string string_type;
typedef std::vector<string_type> string_vector;
typedef std::pair<string_type, string_type> string_pair;
typedef std::vector<string_pair> string_pair_vector;
typedef std::pair<string_type, double> string_score_pair;
typedef std::vector<string_score_pair> string_score_vector;
typedef std::set<string_type> string_set;

typedef void(*CoroutineYieldFunction)();
typedef void(*CoroutineStartFunction)();

static CoroutineYieldFunction YieldFunction = NULL;
static CoroutineStartFunction StartFunction = NULL;

class NFRedisClient
{
public:
    NFRedisClient();

    bool Execute();

    bool ConnectTo(const std::string& ip, const int port, const std::string& auth);

    bool KeepLive();


	/**
	* @brie if you have setted a password for Redis, you much use AUTH cmd to connect to the server than you can use other cmds
	* @param password
	* @return success: true; fail: false
	*/
	bool AUTH(const std::string& auth);

	/**
	* @brie select DB
	* @param DB index
	* @return success: true; fail: false
	*/
	bool SelectDB(int dbnum);

    /*
    ECHO
    PING
    QUIT
    */
    /////////client key//////////////
	/**
	* @brie del a key
	* @param key's name
	* @return success: true; fail: false
	*/
    bool DEL(const std::string& key);

    //NF_SHARE_PTR<NFRedisResult> DUMP(const std::string& key, std::string& out);

	/**
	* @brie Returns if key exists.
	* @param key's name
	* @return true if the key exists, false if the key does not exist.
	*/
    bool EXISTS(const std::string& key);

	/**
	* @brief Set a timeout on key. After the timeout has expired, the key will automatically be deleted
	* @param keys [in] name of key
	* @param seconds [in] the key will be destroy after this second
	* @return The true for set or reset success, false for failed(if key does not exist)
	*/
    bool EXPIRE(const std::string& key, const unsigned int secs);

	/**
	* @brief this function like EXPIRE, which is to set the lfie time for one key
	* the difference is the times of the cmd EXPIREAT is unix timestamp
	* @param keys [in] name of key
	* @param timestamp [in] the key will be destroy after this timestamp
	* @return true if the timeout was set, false if key does not exist.
	*/
    bool EXPIREAT(const std::string& key, const int64_t unixTime);
    //NF_SHARE_PTR<NFRedisResult> KEYS(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> MIGRATE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> MOVE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> OBJECT(const std::string& key);
	/**
	* @brief Remove the existing timeout on key, turning the key from volatile (a key with an expire set) to persistent(a key that will never expire as no timeout is associated).
	* the difference is the times of the cmd EXPIREAT is unix timestamp
	* @param keys [in] name of key
	* @return true if the timeout was removed, false if key does not exist or does not have an associated timeout.
	*/
    bool PERSIST(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> PEXPIRE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> PEXPIREAT(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> PTTL(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> RANDOMKEY(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> RENAME(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> RENAMENX(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> RESTORE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> SORT(const std::string& key);
    /**
	* @brief Returns the remaining time to live of a key that has a timeout
	* @param keys [in] name of key
	* @return TTL in seconds, or a negative value in order to signal an error
	* The command returns -2 if the key does not exist.
	* The command returns -1 if the key exists but has no associated expire.
	*/
    int TTL(const std::string& key);

    /**
	* @brief Returns the string representation of the type of the value stored at key
	* @param keys [in] name of key
	* @return Simple string reply: type of key, or none when key does not exist.
	*/
    std::string TYPE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> SCAN(const std::string& key);


    /////////client String//////////////
    /**
	* @brief If key already exists and is a string, this command appends the value at the end of the string.
    * If key does not exist it is created and set as an empty string
	* @param keys [in] name of key
	* @param value that you want to append
	* @return the length of the string after the append operation.
	*/
    bool APPEND(const std::string& key, const std::string& value, int& length);
    //NF_SHARE_PTR<NFRedisResult> BITCOUNT
    //NF_SHARE_PTR<NFRedisResult> BITOP
    //NF_SHARE_PTR<NFRedisResult> BITFIELD(const std::string& key);

    /**
	* @brief Decrements the number stored at key by one.
    * If the key does not exist, it is set to 0 before performing the operation.
	* @param keys [in] name of key
	* @param value that you want to append
	* @return the value of key after the decrement
    * An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
	*/
    bool DECR(const std::string& key, int64_t& value);

    /**
    * @brief Decrements the number stored at key by decrement.
    * If the key does not exist, it is set to 0 before performing the operation.
    * @param keys [in] name of key
    * @param value that you want to append
    * @return the value of key after the decrement
    * An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
    */
    bool DECRBY(const std::string& key, const int64_t decrement, int64_t& value);

    /**
    * @brief Get the value of key. If the key does not exist the special value "" is returned
    * @param keys [in] name of key
    * @return the value of key, or "" when key does not exist. An error is returned if the value stored at key is not a string
    */
    bool GET(const std::string& key, std::string & value);

    //NF_SHARE_PTR<NFRedisResult> GETBIT(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> GETRANGE(const std::string& key);
    /**
    * @brief Atomically sets key to value and returns the old value stored at key
    * @param keys [in] name of key
    * @return the old value stored at key, or "" when key did not exist.
    */
    bool GETSET(const std::string& key, const std::string& value, std::string& oldValue);

    /**
	* @brief Increments the number stored at key by one.
    * If the key does not exist, it is set to 0 before performing the operation
	* @param keys [in] name of key
	* @return the value of key after the increment
    * An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
	*/
    bool INCR(const std::string& key, int64_t& value);

    /**
	* @brief Increments the number stored at key by increment
    * If the key does not exist, it is set to 0 before performing the operation
	* @param keys [in] name of key
	* @param keys [in] increment
	* @return the value of key after the increment
    * An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
	*/
    bool INCRBY(const std::string& key, const int64_t increment, int64_t& value);

    /**
    * @brief Increment the string representing a floating point number stored at key by the specified increment
    * @param keys [in] name of key
    * @return the value of key after the increment
    * An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as float/double.
    */
    bool INCRBYFLOAT(const std::string& key, const float increment, float& value);

    /**
    * @brief Returns the values of all specified keys.
	* @param keys [in] name of keys
	* @param values [in] values of keys
    * @return list of values at the specified keys.
    * For every key that does not hold a string value or does not exist,
    * the special value "" is returned. Because of this, the operation never fails / if the key is not a string key than that field return ""
    */
    bool MGET(const string_vector& keys, string_vector& values);

    /**
    * @brief Sets the given keys to their respective values, MSET is atomic, so all given keys are set at once
    * @param keys and values [in]
    * @return always OK since MSET can't fail.
    */
    void MSET(const string_pair_vector& values);

    //NF_SHARE_PTR<NFRedisResult> MSETNX(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> PSETEX(const std::string& key);
    /**
    * @brief Set key to hold the string value. If key already holds a value, it is overwritten, regardless of its type
    * @param key [in] name of key
    * @param value [in] value of the key
    * @return true if SET was executed correctly.
    * false is returned if the SET operation was not performed because the user specified the NX or XX option but the condition was not met.
    */
    bool SET(const std::string& key, const std::string& value);

    //NF_SHARE_PTR<NFRedisResult> SETBIT(const std::string& key);
    /**
    * @brief Set key to hold the string value and set key to timeout after a given number of seconds
    * @param key [in] name of key
    * @param value [in] value of the key
    * @param time [in] time that you want set
    * @return true if SETEX was executed correctly, false is returned when seconds is invalid.
    */
    bool SETEX(const std::string& key, const std::string& value, int time);

    /**
    * @brief SET if Not eXists --- Set key to hold string value if key does not exist.
    * @param key [in] name of key
    * @param value [in] value of the key
    * @return true if the key was set, false if the key was not set
    */
    bool SETNX(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> SETRANGE(const std::string& key);

    /**
    * @brief Returns the length of the string value stored at key
	* @param key [in] name of key
	* @param length [out] the length of the string at key
    * @return false when key does not exist or wrong type
    */
    bool STRLEN(const std::string& key, int& length);

    /////////client hash//////////////
    /**
    * @brief Removes the specified fields from the hash stored at key
    * @param field/fields [in] the fields you want to remove
    * @return the number of fields that were removed from the hash, not including specified but non existing fields.
    */
    int HDEL(const std::string& key, const std::string& field);
    int HDEL(const std::string& key, const string_vector& fields);

    /**
    * @brief Returns if field is an existing field in the hash stored at key.
    * @param field [in] the field you want to check
    * @return true if the hash contains field. false if the hash does not contain field, or key does not exist.
    */
    bool HEXISTS(const std::string& key, const std::string& field);

    /**
    * @brief Returns the value associated with field in the hash stored at key.
	* @param field [in] the field you want to get
	* @param value [out] the value you want to get
    * @return true if the hash contains field. false if the hash does not contain field, or key does not exist.
    */
    bool HGET(const std::string& key, const std::string& field, std::string& value);

    /**
    * @brief Returns all field names in the hash stored at key.
    * @param key [in] the name of the key
    * @param values [out] all the key & values of the key
    * @return true when key exist, false when key does not exist.
    */
    bool HGETALL(const std::string& key, std::vector<string_pair>& values);

    /**
    * @brief Increments the number stored at key by increment
    * If the key does not exist, it is set to 0 before performing the operation
    * @param key [in] name of key
    * @param field [in] field
    * @param by [in] increment
    * @return the value of key after the increment
    * An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
    */
    bool HINCRBY(const std::string& key, const std::string& field, const int by, int64_t& value);

    /**
    * @brief Increment the string representing a floating point number stored at key by the specified increment
    * @param key [in] name of key
    * @param field [in] field
    * @param by [in] increment
    * @param value [out] the value of key after the increment
    * @return the value of key after the increment
    * An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as float/double.
    */
    bool HINCRBYFLOAT(const std::string& key, const std::string& field, const float by, float& value);

    /**
    * @brief Returns all field names in the hash stored at key.
    * @param key [in] the name of the key
    * @param fields [out] the fields of the key
    * @return true when key exist, false when key does not exist.
    */
    bool HKEYS(const std::string& key, std::vector<std::string>& fields);


    /**
    * @brief Returns the number of fields contained in the hash stored at key.
	* @param key [in] the name of the key
	* @param number [out] number of fields in the hash
    * @return true when key exist, false when key does not exist.
    */
    bool HLEN(const std::string& key, int& number);

    /**
    * @brief Returns the values associated with the specified fields in the hash stored at key.
    * @param key [in] the name of the key
    * @param field [in] the fields you want to get
    * @param values [out] the values return
    * @return list of values associated with the given fields, in the same order as they are requested.
    */
    bool HMGET(const std::string& key, const string_vector& fields, string_vector& values);

    /**
    * @brief Sets the specified fields to their respective values in the hash stored at key
    * @param key [in] the name of the key
    * @param values [in] the fields/value you want to set
    * @return true if cmd success, false when the key not a hashmap
    * If the key does not exist, a new key holding a hash is created
    */
    bool HMSET(const std::string& key, const std::vector<string_pair>& values);

    /**
    * @brief Sets the specified field to their respective values in the hash stored at key
    * @param key [in] the name of the key
    * @param values [in] the fields/value you want to set
    * @return true if cmd success, false when the key not a hashmap
    * If the key does not exist, a new key holding a hash is created
    */
    bool HSET(const std::string& key, const std::string& field, const std::string& value);

    /**
    * @brief SET if Not eXists --- Sets field in the hash stored at key to value, only if field does not yet exist
    * @param key [in] name of key
    * @param field [in] field of the hashmap
    * @param value [in] value of the field
    * @return true if the key was set, false if the key was not set(maybe not a hashmap key)
    */
    bool HSETNX(const std::string& key, const std::string& field, const std::string& value);

    /**
    * @brief Returns all values in the hash stored at key.
    * @param key [in] name of key
    * @param values [out] fields/values that you want to know
    * @return false when key does not exist or not a hashmap key
    */
    bool HVALS(const std::string& key, string_vector& values);
    //NF_SHARE_PTR<NFRedisResult> HSCAN(const std::string& key, const std::string& field);

    /**
    * @brief Returns the length of the string value stored at key
    * @param key [in] name of key
    * @param field [in] field that you want to know
    * @param length [out] the length of the string at field, or 0 when field does not exist.
    * @return true when cmd success, false when key does not exist or not a list key.
    */
    bool HSTRLEN(const std::string& key, const std::string& field, int& length);

    /////////client list//////////////

    //NF_SHARE_PTR<NFRedisResult> BLPOP(const std::string& key, string_vector& values);
    //NF_SHARE_PTR<NFRedisResult> BRPOP(const std::string& key, string_vector& values);
    //NF_SHARE_PTR<NFRedisResult> BRPOPLPUSH(const std::string& key, string_vector& values);
    /**
    * @brief Returns the element at index index in the list stored at key
    * @param key [in] name of key
    * @param index [in] index that you want to know
    * @param value [out] th value that you got
    * @return true when cmd success, false when key does not exist or not a list key.
    */
    bool LINDEX(const std::string& key, const int index, std::string& value);
    //NF_SHARE_PTR<NFRedisResult> LINSERT(const std::string& key, const std::string& value1, const std::string& value2);

    /**
    * @brief Returns the length of the list stored at key
    * @param key [in] name of key
    * @param length [out] the length that you got
    * @return true when cmd success, false when key does not exist or not a list key.
    */
    bool LLEN(const std::string& key, int& length);

    /**
    * @brief Removes and returns the first element of the list stored at key.
    * @param key [in] name of key
    * @param value [out] the value of the first element
    * @return true when cmd success, false when key does not exist or not a list key.
    */
    bool LPOP(const std::string& key, std::string& value);

    /**
    * @brief Insert all the specified values at the head of the list stored at key
    * @param key [in] name of key
    * @param value [in] the value that you want to push to the head
    * @return length of the list when cmd success, 0 when key does not a list key.
    */
    int LPUSH(const std::string& key, const std::string& value);

    /**
    * @brief Insert all the specified values at the head of the list stored at key
    * only if key already exists and holds a list.
    * @param key [in] name of key
    * @param value [in] the value that you want to push to the head
    * @return length of the list when cmd success, 0 when key does not a list key.
    */
    int LPUSHX(const std::string& key, const std::string& value);


    /**
    * @brief Returns the specified elements of the list stored at key by start(included) and end(included)
    * @param key [in] name of key
    * @param start [in]
    * @param end [in]
    * @param values [out] the value that you want to get
    * @return true when cmd success, false when key does not exist or dose not a list key.
    */
    bool LRANGE(const std::string& key, const int start, const int end, string_vector& values);
    //NF_SHARE_PTR<NFRedisResult> LREM(const std::string& key, string_vector& values);

    /**
    * @brief Sets the list element at index to value
    * @param key [in] name of key
    * @param index [in] the index of this list
    * @param value [in] the value that you want set
    * @return true when cmd success, false when key dose not a list key.
    */
    bool LSET(const std::string& key, const int index, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> LTRIM(const std::string& key, string_vector& values);

    /**
    * @brief Removes and returns the last element of the list stored at key.
    * @param key [in] name of key
    * @param value [out] the value of the last element
    * @return true when cmd success, false when key does not exist or not a list key.
    */
    bool RPOP(const std::string& key, std::string& value);
    //NF_SHARE_PTR<NFRedisResult> RPOPLPUSH(const std::string& key, string_vector& values);

    /**
   * @brief Insert all the specified values at the last of the list stored at key
   * @param key [in] name of key
   * @param value [in] the value that you want to push to the head
   * @return length of the list when cmd success, 0 when key does not a list key.
   */
    int RPUSH(const std::string& key, const std::string& value);

    /**
    * @brief Inserts value at the tail of the list stored at key, only if key already exists and holds a list
    * @param key [in] name of key
    * @param value [in] the value that you want push
    * @return true when cmd success, false when key does not exist or dose not a list key.
    */
    int RPUSHX(const std::string& key, const std::string& value);

    /////////client set//////////////
    /*
    NF_SHARE_PTR<NFRedisResult> SADD(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SCARD(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SDIFF(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SDIFFSTORE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SINTER(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SINTERSTORE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SISMEMBER(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SMEMBERS(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SMOVE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SPOP(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SRANDMEMBER(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SREM(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SUNION(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SUNIONSTORE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SSCAN(const std::string& key, const std::string& value);
*/

    /////////client SortedSet//////////////
    /**
    * @brief Adds all the specified members with the specified scores to the sorted set stored at key
    * @param key [in] name of key
    * @param member [in]
    * @param score [in]
    * @return return the number of elements added to the sorted sets when cmd success, false when key dose not a z key.
    */
    int ZADD(const std::string& key, const std::string& member, const double score);

    /**
    * @brief Returns the number of elements of the sorted set stored at key.
    * @param key [in] name of key
    * @return return the number( of elements) of the sorted set, or 0 if key does not exist or not a z key
    */
    int ZCARD(const std::string& key);

    /**
    * @brief Returns the number of elements in the sorted set at key with a score between min and max.
    * @param key [in] name of key
    * @param start [in]
    * @param end [in]
    * @return the number of elements in the specified score range, or 0 if key does not exist or not a z key
    */
    int ZCOUNT(const std::string& key, const double start, const double end);

    /**
    * @brief Increments the score of member in the sorted set stored at key by increment
    * If member does not exist in the sorted set, it is added with increment as its score (as if its previous score was 0.0)
    * @param key [in] name of key
    * @param member [in]
    * @param score [in]
    * @param newScore [out] the new score of member
    * @return the value of key after the increment
    * An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as float/double.
    */
    bool ZINCRBY(const std::string& key, const std::string & member, const double score, double& newScore);

    /**
    * @brief Returns the specified range of elements in the sorted set stored at key
    * @param key [in] name of key
    * @param start [in]
    * @param end [in]
    * @param values [out] the members of this range
    * @return true when cmd success, false when key does not exist or not a z key.
    */
    bool ZRANGE(const std::string& key, const int start, const int end, string_vector& values);

    /**
    * @brief Returns all the elements in the sorted set at key with a score between min and max
    * (including elements with score equal to min or max), the elements are considered to be ordered from low to high scores.
    * @param key [in] name of key
    * @param start [in]
    * @param end [in]
    * @param values [out] the members of this range
    * @return true when cmd success, false when key does not exist or not a z key.
    */
    bool ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_vector& values);

    /**
    * @brief Returns the rank of member in the sorted set stored at key, with the scores ordered from low to high
    * @param key [in] name of key
    * @param member [in] the members of this range
    * @param rank [out] the rank of this member
    * @return true if member exists in the sorted set, false when member does not exist or not a z key.
    */
    bool ZRANK(const std::string & key, const std::string & member, int& rank);

    /**
    * @brief Removes the specified members from the sorted set stored at key. Non existing members are ignored.
    * @param key [in] name of key
    * @param member [in] the members of this range
    * @return true if member exists in the sorted set and removed success, false when member does not exist or not a z key.
    */
    bool ZREM(const std::string& key, const std::string& member);

    /**
    * @brief Removes all elements in the sorted set stored at key with rank between start and stop
    * @param key [in] name of key
    * @param start [in]
    * @param end [in]
    * @return true if cmd removed success, false when the key is not a z key.
    */
    bool ZREMRANGEBYRANK(const std::string& key, const int start, const int end);

    /**
    * @brief Removes all elements in the sorted set stored at key with a score between min and max (inclusive).
    * @param key [in] name of key
    * @param min [in]
    * @param max [in]
    * @return true if cmd removed success, false when the key is not a z key.
    */
    bool ZREMRANGEBYSCORE(const std::string& key, const double min, const double max);

    /**
    * @brief Returns the specified range of elements in the sorted set stored at key.
    * The elements are considered to be ordered from the highest to the lowest score.
    * @param key [in] name of key
    * @param start [in]
    * @param end [in]
    * @param values [out] the members of this range
    * @return true when cmd success, false when key does not exist or not a z key.
    */
    bool ZREVRANGE(const std::string& key, const int start, const int end, string_vector& values);

    /**
    * @brief Returns all the elements in the sorted set at key with a score between max and min
    * (including elements with score equal to max or min)
    * @param key [in] name of key
    * @param start [in]
    * @param end [in]
    * @param values [out] the members of this range
    * @return true when cmd success, false when key does not exist or not a z key.
    */
    bool ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_vector& values);


    /**
    * @brief Returns the rank of member in the sorted set stored at key, with the scores ordered from high to low.
    * @param key [in] name of key
    * @param member [in] the members of this range
    * @param rank [out] the rank of this member
    * @return true if member is exists in the sorted set, false when member does not exist or not a z key.
    */
    bool ZREVRANK(const std::string& key, const std::string& member, int& rank);

    /**
    * @brief Returns the score of member in the sorted set at key.
    * @param key [in] name of key
    * @param member [in] the members of this range
    * @param rank [out] the rank of this member
    * @return true if member is exists in the sorted set, false when member does not exist or not a z key.
    */
    bool ZSCORE(const std::string& key, const std::string& member, double& score);
    //NF_SHARE_PTR<NFRedisResult> ZUNIONSTORE(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZINTERSTORE(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZSCAN(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZRANGEBYLEX(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZLEXCOUNT(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZREMRANGEBYLEX(const std::string& key, const std::string& value);

    /////////client server//////////////
    /**
    * @brief Removes all data of all DB
    */
    void FLUSHALL();

    /**
    * @brief Removes all the keys of current DB
    */
    void FLUSHDB();

private:

	NF_SHARE_PTR<NFRedisResult> GetUnuseResult();
	NF_SHARE_PTR<NFRedisResult> BuildSendCmd(const NFRedisCommand& cmd);
	void WaitingResult(NF_SHARE_PTR<NFRedisResult> xRedisResult);

private:
	std::list<NF_SHARE_PTR<NFRedisResult>> mlCmdResultList;
	std::list<NF_SHARE_PTR<NFRedisResult>> mlUnusedResultList;

private:
    NFRedisClientSocket* m_pRedisClientSocket;
};


#endif //NFREDISPLUGIN_NFREDISCLIENT_H
