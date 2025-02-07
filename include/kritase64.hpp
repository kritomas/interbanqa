/*
 *	MIT License
 *
 *	Copyright (c) 2024 kritomas
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 *
 *	Source code: https://gitlab.com/kritomas/kritase64
 *
 *	This project implements the base64 encoding (RFC 4648).
 *	The base64 alphabet is as follows:
 *	ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
 *	Alternative: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_
 *	Pad: =
 */

#ifndef KRITASE64_HPP
#define KRITASE64_HPP

#include <exception>
#include <sstream>
#include <stdexcept>
#include <stdint.h>

namespace kritase64
{
	enum Base64ErrorTypes { ERROR_SUCCESS = 0, ERROR_UNKNOWN = -1, ERROR_INVALID_BASE64_STRING = 1, ERROR_INVALID_BASE64_CHARACTER, ERROR_VALUE_OUT_OF_RANGE };

	class Base64Exception : public std::runtime_error
	{
	public:
		Base64ErrorTypes type;

		Base64Exception(Base64ErrorTypes type, std::string message);
	};

	typedef std::basic_string<uint8_t> Buffer;

	/**
	 * Checks, whether a given string is a valid base64 string.
	 *
	 * @param string The string to check.
	 * @return true, if the given string is a valid base64 string, false otherwise.
	 */
	bool check(const std::string& string);

	/**
	 * Encodes a buffer as a base64 string.
	 *
	 * @param buffer Pointer to the beginning of the buffer.
	 * @param size Size of the buffer.
	 * @param use_alternative Whether to use the alternative ("URL and Filename safe") base64 alphabet or not.
	 * @return The resulting base64 string.
	 */
	std::string encode(const uint8_t* buffer, size_t size, bool use_alternative = false);
	/**
	 * Encodes a buffer as a base64 string.
	 *
	 * @param buffsr The buffer to encode.
	 * @param use_alternative Whether to use the alternative ("URL and Filename safe") base64 alphabet or not.
	 * @return The resulting base64 string.
	 */
	std::string encode(const Buffer& buffer, bool use_alternative = false);
	/**
	 * Encodes a string as a base64 string.
	 *
	 * @param string The string to encode.
	 * @param use_alternative Whether to use the alternative ("URL and Filename safe") base64 alphabet or not.
	 * @return The resulting base64 string.
	 */
	std::string encode(const std::string& string, bool use_alternative = false);

	/**
	 * Decodes a base64 string to a buffer.
	 *
	 * @param string The base64 string to decode.
	 * @param ignore_all Whether to ignore all illegal characters or not.
	 * @return The resulting buffer.
	 */
	Buffer decode(std::string string, bool ignore_all = false);
	/**
	 * Decodes a base64 string to a normal string.
	 *
	 * @param string The base64 string to decode.
	 * @param ignore_all Whether to ignore all illegal characters or not.
	 * @return The resulting nornmal string.
	 */
	std::string decodeToString(const std::string& string, bool ignore_all = false);

	typedef int Base64Mode;
	enum Base64Modes
	{
		/// Whether to use the alternative ("URL and Filename safe") base64 alphabet or not.
		MODE_USEALTERNATIVE = 1 << 0,
		/// Whether to ignore all illegal characters or just a select few whitespace characters.
		MODE_IGNOREALL = 1 << 1
	};

	class Stream : public std::stringstream
	{
	protected:
		Base64Mode mode = 0;

		std::string str() const; // Hiding base method

	public:
		/**
		 * @param base64 The default base64 string to open.
		 * @param openmode The std::ios::openmode to use by the underlying iostream.
		 * @param base64mode The modes to use for encoding/decoding. Any of Base64Modes, or'd together.
		 */
		Stream(const std::string& base64 = "", std::ios_base::openmode openmode = std::ios::in | std::ios::out, Base64Mode mode = 0);

		/**
		 * @return The raw, unencoded buffer.
		 */
		Buffer buffer() const;
		/**
		 * Sets the raw, unencoded buffer.
		 *
		 * @param buffer The buffer to set.
		 */
		void buffer(const Buffer& buffer);

		/**
		 * @return The base64 string representing the data in this stream.
		 */
		std::string base64() const;
		/**
		 * Loads the specified base64 string into the stream.
		 *
		 * @param base64 The base64 string to load.
		 */
		void base64(const std::string& base64);
	};
	class Istream : public std::istringstream
	{
	protected:
		Base64Mode mode = 0;

		std::string str() const; // Hiding base method

	public:
		/**
		 * @param base64 The default base64 string to open.
		 * @param openmode The std::ios::openmode to use by the underlying iostream.
		 * @param base64mode The modes to use for encoding/decoding. Any of Base64Modes, or'd together.
		 */
		Istream(const std::string& base64 = "", std::ios_base::openmode openmode = std::ios::in, Base64Mode mode = 0);

		/**
		 * @return The raw, unencoded buffer.
		 */
		Buffer buffer() const;
		/**
		 * Sets the raw, unencoded buffer.
		 *
		 * @param buffer The buffer to set.
		 */
		void buffer(const Buffer& buffer);

		/**
		 * @return The base64 string representing the data in this stream.
		 */
		std::string base64() const;
		/**
		 * Loads the specified base64 string into the stream.
		 *
		 * @param base64 The base64 string to load.
		 */
		void base64(const std::string& base64);
	};
	class Ostream : public std::ostringstream
	{
	protected:
		Base64Mode mode = 0;

		std::string str() const; // Hiding base method

	public:
		/**
		 * @param base64 The default base64 string to open.
		 * @param openmode The std::ios::openmode to use by the underlying iostream.
		 * @param base64mode The modes to use for encoding/decoding. Any of Base64Modes, or'd together.
		 */
		Ostream(const std::string& base64 = "", std::ios_base::openmode openmode = std::ios::out, Base64Mode mode = 0);

		/**
		 * @return The raw, unencoded buffer.
		 */
		Buffer buffer() const;
		/**
		 * Sets the raw, unencoded buffer.
		 *
		 * @param buffer The buffer to set.
		 */
		void buffer(const Buffer& buffer);

		/**
		 * @return The base64 string representing the data in this stream.
		 */
		std::string base64() const;
		/**
		 * Loads the specified base64 string into the stream.
		 *
		 * @param base64 The base64 string to load.
		 */
		void base64(const std::string& base64);
	};
};

#endif
