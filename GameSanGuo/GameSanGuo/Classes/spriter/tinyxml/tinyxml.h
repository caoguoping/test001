/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2002 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/


#ifndef TINYXML_INCLUDED
#define TINYXML_INCLUDED

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Help out windows:
#if defined( _DEBUG ) && !defined( DEBUG )
#define DEBUG
#endif

#ifdef TTiXml_USE_STL
	#include <string>
 	#include <iostream>
	#define TTiXml_STRING	std::string
	#define TTiXml_ISTREAM	std::istream
	#define TTiXml_OSTREAM	std::ostream
#else
	#include "tinystr.h"
	#define TTiXml_STRING	TTiXmlString
	#define TTiXml_OSTREAM	TTiXmlOutStream
#endif

// Deprecated library function hell. Compilers want to use the
// new safe versions. This probably doesn't fully address the problem,
// but it gets closer. There are too many compilers for me to fully
// test. If you get compilation troubles, undefine TTiXml_SAFE

#define TTiXml_SAFE		// TinyXml isn't fully buffer overrun protected, safe code. This is work in progress.
#ifdef TTiXml_SAFE
	#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
		// Microsoft visual studio, version 2005 and higher.
		#define TTiXml_SNPRINTF _snprintf_s
		#define TTiXml_SNSCANF  _snscanf_s
	#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
		// Microsoft visual studio, version 6 and higher.
		//#pragma message( "Using _sn* functions." )
		#define TTiXml_SNPRINTF _snprintf
		#define TTiXml_SNSCANF  _snscanf
	#elif defined(__GNUC__) && (__GNUC__ >= 3 )
		// GCC version 3 and higher.s
		//#warning( "Using sn* functions." )
		#define TTiXml_SNPRINTF snprintf
		#define TTiXml_SNSCANF  snscanf
	#endif
#endif	

class TTiXmlDocument;
class TTiXmlElement;
class TTiXmlComment;
class TTiXmlUnknown;
class TTiXmlAttribute;
class TTiXmlText;
class TTiXmlDeclaration;
class TTiXmlParsingData;

const int TTiXml_MAJOR_VERSION = 2;
const int TTiXml_MINOR_VERSION = 4;
const int TTiXml_PATCH_VERSION = 3;

/*	Internal structure for tracking location of items 
	in the XML file.
*/
struct TTiXmlCursor
{
	TTiXmlCursor()		{ Clear(); }
	void Clear()		{ row = col = -1; }

	int row;	// 0 based.
	int col;	// 0 based.
};


// Only used by Attribute::Query functions
enum 
{ 
	TTiXml_SUCCESS,
	TTiXml_NO_ATTRIBUTE,
	TTiXml_WRONG_TYPE
};


// Used by the parsing routines.
enum TTiXmlEncoding
{
	TTiXml_ENCODING_UNKNOWN,
	TTiXml_ENCODING_UTF8,
	TTiXml_ENCODING_LEGACY
};

const TTiXmlEncoding TTiXml_DEFAULT_ENCODING = TTiXml_ENCODING_UNKNOWN;

/** TTiXmlBase is a base class for every class in TinyXml.
	It does little except to establish that TinyXml classes
	can be printed and provide some utility functions.

	In XML, the document and elements can contain
	other elements and other types of nodes.

	@verbatim
	A Document can contain:	Element	(container or leaf)
							Comment (leaf)
							Unknown (leaf)
							Declaration( leaf )

	An Element can contain:	Element (container or leaf)
							Text	(leaf)
							Attributes (not on tree)
							Comment (leaf)
							Unknown (leaf)

	A Decleration contains: Attributes (not on tree)
	@endverbatim
*/
class TTiXmlBase
{
	friend class TTiXmlNode;
	friend class TTiXmlElement;
	friend class TTiXmlDocument;

public:
	TTiXmlBase()	:	userData(0) {}
	virtual ~TTiXmlBase()					{}

	/**	All TinyXml classes can print themselves to a filestream.
		This is a formatted print, and will insert tabs and newlines.
		
		(For an unformatted stream, use the << operator.)
	*/
	virtual void Print( FILE* cfile, int depth ) const = 0;

	/**	The world does not agree on whether white space should be kept or
		not. In order to make everyone happy, these global, static functions
		are provided to set whether or not TinyXml will condense all white space
		into a single space or not. The default is to condense. Note changing this
		values is not thread safe.
	*/
	static void SetCondenseWhiteSpace( bool condense )		{ condenseWhiteSpace = condense; }

	/// Return the current white space setting.
	static bool IsWhiteSpaceCondensed()						{ return condenseWhiteSpace; }

	/** Return the position, in the original source file, of this node or attribute.
		The row and column are 1-based. (That is the first row and first column is
		1,1). If the returns values are 0 or less, then the parser does not have
		a row and column value.

		Generally, the row and column value will be set when the TTiXmlDocument::Load(),
		TTiXmlDocument::LoadFile(), or any TTiXmlNode::Parse() is called. It will NOT be set
		when the DOM was created from operator>>.

		The values reflect the initial load. Once the DOM is modified programmatically
		(by adding or changing nodes and attributes) the new values will NOT update to
		reflect changes in the document.

		There is a minor performance cost to computing the row and column. Computation
		can be disabled if TTiXmlDocument::SetTabSize() is called with 0 as the value.

		@sa TTiXmlDocument::SetTabSize()
	*/
	int Row() const			{ return location.row + 1; }
	int Column() const		{ return location.col + 1; }	///< See Row()

	void  SetUserData( void* user )			{ userData = user; }
	void* GetUserData()						{ return userData; }

	// Table that returs, for a given lead byte, the total number of bytes
	// in the UTF-8 sequence.
	static const int utf8ByteTable[256];

	virtual const char* Parse(	const char* p, 
								TTiXmlParsingData* data, 
								TTiXmlEncoding encoding /*= TTiXml_ENCODING_UNKNOWN */ ) = 0;

	enum
	{
		TTiXml_NO_ERROR = 0,
		TTiXml_ERROR,
		TTiXml_ERROR_OPENING_FILE,
		TTiXml_ERROR_OUT_OF_MEMORY,
		TTiXml_ERROR_PARSING_ELEMENT,
		TTiXml_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		TTiXml_ERROR_READING_ELEMENT_VALUE,
		TTiXml_ERROR_READING_ATTRIBUTES,
		TTiXml_ERROR_PARSING_EMPTY,
		TTiXml_ERROR_READING_END_TAG,
		TTiXml_ERROR_PARSING_UNKNOWN,
		TTiXml_ERROR_PARSING_COMMENT,
		TTiXml_ERROR_PARSING_DECLARATION,
		TTiXml_ERROR_DOCUMENT_EMPTY,
		TTiXml_ERROR_EMBEDDED_NULL,
		TTiXml_ERROR_PARSING_CDATA,

		TTiXml_ERROR_STRING_COUNT
	};

protected:

	// See STL_STRING_BUG
	// Utility class to overcome a bug.
	class StringToBuffer
	{
	  public:
		StringToBuffer( const TTiXml_STRING& str );
		~StringToBuffer();
		char* buffer;
	};

	static const char*	SkipWhiteSpace( const char*, TTiXmlEncoding encoding );
	inline static bool	IsWhiteSpace( char c )		
	{ 
		return ( isspace( (unsigned char) c ) || c == '\n' || c == '\r' ); 
	}
	inline static bool	IsWhiteSpace( int c )
	{
		if ( c < 256 )
			return IsWhiteSpace( (char) c );
		return false;	// Again, only truly correct for English/Latin...but usually works.
	}

	virtual void StreamOut (TTiXml_OSTREAM *) const = 0;

	#ifdef TTiXml_USE_STL
	    static bool	StreamWhiteSpace( TTiXml_ISTREAM * in, TTiXml_STRING * tag );
	    static bool StreamTo( TTiXml_ISTREAM * in, int character, TTiXml_STRING * tag );
	#endif

	/*	Reads an XML name into the string provided. Returns
		a pointer just past the last character of the name,
		or 0 if the function has an error.
	*/
	static const char* ReadName( const char* p, TTiXml_STRING* name, TTiXmlEncoding encoding );

	/*	Reads text. Returns a pointer past the given end tag.
		Wickedly complex options, but it keeps the (sensitive) code in one place.
	*/
	static const char* ReadText(	const char* in,				// where to start
									TTiXml_STRING* text,			// the string read
									bool ignoreWhiteSpace,		// whether to keep the white space
									const char* endTag,			// what ends this text
									bool ignoreCase,			// whether to ignore case in the end tag
									TTiXmlEncoding encoding );	// the current encoding

	// If an entity has been found, transform it into a character.
	static const char* GetEntity( const char* in, char* value, int* length, TTiXmlEncoding encoding );

	// Get a character, while interpreting entities.
	// The length can be from 0 to 4 bytes.
	inline static const char* GetChar( const char* p, char* _value, int* length, TTiXmlEncoding encoding )
	{
		assert( p );
		if ( encoding == TTiXml_ENCODING_UTF8 )
		{
			*length = utf8ByteTable[ *((unsigned char*)p) ];
			assert( *length >= 0 && *length < 5 );
		}
		else
		{
			*length = 1;
		}

		if ( *length == 1 )
		{
			if ( *p == '&' )
				return GetEntity( p, _value, length, encoding );
			*_value = *p;
			return p+1;
		}
		else if ( *length )
		{
			//strncpy( _value, p, *length );	// lots of compilers don't like this function (unsafe),
												// and the null terminator isn't needed
			for( int i=0; p[i] && i<*length; ++i ) {
				_value[i] = p[i];
			}
			return p + (*length);
		}
		else
		{
			// Not valid text.
			return 0;
		}
	}

	// Puts a string to a stream, expanding entities as it goes.
	// Note this should not contian the '<', '>', etc, or they will be transformed into entities!
	static void PutString( const TTiXml_STRING& str, TTiXml_OSTREAM* out );

	static void PutString( const TTiXml_STRING& str, TTiXml_STRING* out );

	// Return true if the next characters in the stream are any of the endTag sequences.
	// Ignore case only works for english, and should only be relied on when comparing
	// to English words: StringEqual( p, "version", true ) is fine.
	static bool StringEqual(	const char* p,
								const char* endTag,
								bool ignoreCase,
								TTiXmlEncoding encoding );

	static const char* errorString[ TTiXml_ERROR_STRING_COUNT ];

	TTiXmlCursor location;

    /// Field containing a generic user pointer
	void*			userData;
	
	// None of these methods are reliable for any language except English.
	// Good for approximation, not great for accuracy.
	static int IsAlpha( unsigned char anyByte, TTiXmlEncoding encoding );
	static int IsAlphaNum( unsigned char anyByte, TTiXmlEncoding encoding );
	inline static int ToLower( int v, TTiXmlEncoding encoding )
	{
		if ( encoding == TTiXml_ENCODING_UTF8 )
		{
			if ( v < 128 ) return tolower( v );
			return v;
		}
		else
		{
			return tolower( v );
		}
	}
	static void ConvertUTF32ToUTF8( unsigned long input, char* output, int* length );

private:
	TTiXmlBase( const TTiXmlBase& );				// not implemented.
	void operator=( const TTiXmlBase& base );	// not allowed.

	struct Entity
	{
		const char*     str;
		unsigned int	strLength;
		char		    chr;
	};
	enum
	{
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6

	};
	static Entity entity[ NUM_ENTITY ];
	static bool condenseWhiteSpace;
};


/** The parent class for everything in the Document Object Model.
	(Except for attributes).
	Nodes have siblings, a parent, and children. A node can be
	in a document, or stand on its own. The type of a TTiXmlNode
	can be queried, and it can be cast to its more defined type.
*/
class TTiXmlNode : public TTiXmlBase
{
	friend class TTiXmlDocument;
	friend class TTiXmlElement;

public:
	#ifdef TTiXml_USE_STL	

	    /** An input stream operator, for every class. Tolerant of newlines and
		    formatting, but doesn't expect them.
	    */
	    friend std::istream& operator >> (std::istream& in, TTiXmlNode& base);

	    /** An output stream operator, for every class. Note that this outputs
		    without any newlines or formatting, as opposed to Print(), which
		    includes tabs and new lines.

		    The operator<< and operator>> are not completely symmetric. Writing
		    a node to a stream is very well defined. You'll get a nice stream
		    of output, without any extra whitespace or newlines.
		    
		    But reading is not as well defined. (As it always is.) If you create
		    a TTiXmlElement (for example) and read that from an input stream,
		    the text needs to define an element or junk will result. This is
		    true of all input streams, but it's worth keeping in mind.

		    A TTiXmlDocument will read nodes until it reads a root element, and
			all the children of that root element.
	    */	
	    friend std::ostream& operator<< (std::ostream& out, const TTiXmlNode& base);

		/// Appends the XML node or attribute to a std::string.
		friend std::string& operator<< (std::string& out, const TTiXmlNode& base );

	#else
	    // Used internally, not part of the public API.
	    friend TTiXml_OSTREAM& operator<< (TTiXml_OSTREAM& out, const TTiXmlNode& base);
	#endif

	/** The types of XML nodes supported by TinyXml. (All the
			unsupported types are picked up by UNKNOWN.)
	*/
	enum NodeType
	{
		DOCUMENT,
		ELEMENT,
		COMMENT,
		UNKNOWN,
		TEXT,
		DECLARATION,
		TYPECOUNT
	};

	virtual ~TTiXmlNode();

	/** The meaning of 'value' changes for the specific type of
		TTiXmlNode.
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim

		The subclasses will wrap this function.
	*/
	const char *Value() const { return value.c_str (); }

    #ifdef TTiXml_USE_STL
	/** Return Value() as a std::string. If you only use STL,
	    this is more efficient than calling Value().
		Only available in STL mode.
	*/
	const std::string& ValueStr() const { return value; }
	#endif

	/** Changes the value of the node. Defined as:
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim
	*/
	void SetValue(const char * _value) { value = _value;}

    #ifdef TTiXml_USE_STL
	/// STL std::string form.
	void SetValue( const std::string& _value )	{ value = _value; }
	#endif

	/// Delete all the children of this node. Does not affect 'this'.
	void Clear();

	/// One step up the DOM.
	TTiXmlNode* Parent()							{ return parent; }
	const TTiXmlNode* Parent() const				{ return parent; }

	const TTiXmlNode* FirstChild()	const	{ return firstChild; }		///< The first child of this node. Will be null if there are no children.
	TTiXmlNode* FirstChild()					{ return firstChild; }
	const TTiXmlNode* FirstChild( const char * value ) const;			///< The first child of this node with the matching 'value'. Will be null if none found.
	TTiXmlNode* FirstChild( const char * value );						///< The first child of this node with the matching 'value'. Will be null if none found.

	const TTiXmlNode* LastChild() const	{ return lastChild; }		/// The last child of this node. Will be null if there are no children.
	TTiXmlNode* LastChild()	{ return lastChild; }
	const TTiXmlNode* LastChild( const char * value ) const;			/// The last child of this node matching 'value'. Will be null if there are no children.
	TTiXmlNode* LastChild( const char * value );	

    #ifdef TTiXml_USE_STL
	const TTiXmlNode* FirstChild( const std::string& _value ) const	{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
	TTiXmlNode* FirstChild( const std::string& _value )				{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
	const TTiXmlNode* LastChild( const std::string& _value ) const	{	return LastChild (_value.c_str ());	}	///< STL std::string form.
	TTiXmlNode* LastChild( const std::string& _value )				{	return LastChild (_value.c_str ());	}	///< STL std::string form.
	#endif

	/** An alternate way to walk the children of a node.
		One way to iterate over nodes is:
		@verbatim
			for( child = parent->FirstChild(); child; child = child->NextSibling() )
		@endverbatim

		IterateChildren does the same thing with the syntax:
		@verbatim
			child = 0;
			while( child = parent->IterateChildren( child ) )
		@endverbatim

		IterateChildren takes the previous child as input and finds
		the next one. If the previous child is null, it returns the
		first. IterateChildren will return null when done.
	*/
	const TTiXmlNode* IterateChildren( const TTiXmlNode* previous ) const;
	TTiXmlNode* IterateChildren( TTiXmlNode* previous );

	/// This flavor of IterateChildren searches for children with a particular 'value'
	const TTiXmlNode* IterateChildren( const char * value, const TTiXmlNode* previous ) const;
	TTiXmlNode* IterateChildren( const char * value, TTiXmlNode* previous );

    #ifdef TTiXml_USE_STL
	const TTiXmlNode* IterateChildren( const std::string& _value, const TTiXmlNode* previous ) const	{	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
	TTiXmlNode* IterateChildren( const std::string& _value, TTiXmlNode* previous ) {	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
	#endif

	/** Add a new node related to this. Adds a child past the LastChild.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TTiXmlNode* InsertEndChild( const TTiXmlNode& addThis );


	/** Add a new node related to this. Adds a child past the LastChild.

		NOTE: the node to be added is passed by pointer, and will be
		henceforth owned (and deleted) by tinyXml. This method is efficient
		and avoids an extra copy, but should be used with care as it
		uses a different memory model than the other insert functions.

		@sa InsertEndChild
	*/
	TTiXmlNode* LinkEndChild( TTiXmlNode* addThis );

	/** Add a new node related to this. Adds a child before the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TTiXmlNode* InsertBeforeChild( TTiXmlNode* beforeThis, const TTiXmlNode& addThis );

	/** Add a new node related to this. Adds a child after the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TTiXmlNode* InsertAfterChild(  TTiXmlNode* afterThis, const TTiXmlNode& addThis );

	/** Replace a child of this node.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TTiXmlNode* ReplaceChild( TTiXmlNode* replaceThis, const TTiXmlNode& withThis );

	/// Delete a child of this node.
	bool RemoveChild( TTiXmlNode* removeThis );

	/// Navigate to a sibling node.
	const TTiXmlNode* PreviousSibling() const			{ return prev; }
	TTiXmlNode* PreviousSibling()						{ return prev; }

	/// Navigate to a sibling node.
	const TTiXmlNode* PreviousSibling( const char * ) const;
	TTiXmlNode* PreviousSibling( const char * );

    #ifdef TTiXml_USE_STL
	const TTiXmlNode* PreviousSibling( const std::string& _value ) const	{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
	TTiXmlNode* PreviousSibling( const std::string& _value ) 			{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
	const TTiXmlNode* NextSibling( const std::string& _value) const		{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
	TTiXmlNode* NextSibling( const std::string& _value) 					{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
	#endif

	/// Navigate to a sibling node.
	const TTiXmlNode* NextSibling() const				{ return next; }
	TTiXmlNode* NextSibling()							{ return next; }

	/// Navigate to a sibling node with the given 'value'.
	const TTiXmlNode* NextSibling( const char * ) const;
	TTiXmlNode* NextSibling( const char * );

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	const TTiXmlElement* NextSiblingElement() const;
	TTiXmlElement* NextSiblingElement();

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	const TTiXmlElement* NextSiblingElement( const char * ) const;
	TTiXmlElement* NextSiblingElement( const char * );

    #ifdef TTiXml_USE_STL
	const TTiXmlElement* NextSiblingElement( const std::string& _value) const	{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
	TTiXmlElement* NextSiblingElement( const std::string& _value)				{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
	#endif

	/// Convenience function to get through elements.
	const TTiXmlElement* FirstChildElement()	const;
	TTiXmlElement* FirstChildElement();

	/// Convenience function to get through elements.
	const TTiXmlElement* FirstChildElement( const char * value ) const;
	TTiXmlElement* FirstChildElement( const char * value );

    #ifdef TTiXml_USE_STL
	const TTiXmlElement* FirstChildElement( const std::string& _value ) const	{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
	TTiXmlElement* FirstChildElement( const std::string& _value )				{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
	#endif

	/** Query the type (as an enumerated value, above) of this node.
		The possible types are: DOCUMENT, ELEMENT, COMMENT,
								UNKNOWN, TEXT, and DECLARATION.
	*/
	int Type() const	{ return type; }

	/** Return a pointer to the Document this node lives in.
		Returns null if not in a document.
	*/
	const TTiXmlDocument* GetDocument() const;
	TTiXmlDocument* GetDocument();

	/// Returns true if this node has no children.
	bool NoChildren() const						{ return !firstChild; }

	virtual const TTiXmlDocument*    ToDocument()    const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TTiXmlElement*     ToElement()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TTiXmlComment*     ToComment()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TTiXmlUnknown*     ToUnknown()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TTiXmlText*        ToText()        const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TTiXmlDeclaration* ToDeclaration() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	virtual TTiXmlDocument*          ToDocument()    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TTiXmlElement*           ToElement()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TTiXmlComment*           ToComment()     { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TTiXmlUnknown*           ToUnknown()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TTiXmlText*	            ToText()        { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TTiXmlDeclaration*       ToDeclaration() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	/** Create an exact duplicate of this node and return it. The memory must be deleted
		by the caller. 
	*/
	virtual TTiXmlNode* Clone() const = 0;

protected:
	TTiXmlNode( NodeType _type );

	// Copy to the allocated object. Shared functionality between Clone, Copy constructor,
	// and the assignment operator.
	void CopyTo( TTiXmlNode* target ) const;

	#ifdef TTiXml_USE_STL
	    // The real work of the input operator.
	    virtual void StreamIn( TTiXml_ISTREAM* in, TTiXml_STRING* tag ) = 0;
	#endif

	// Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	TTiXmlNode* Identify( const char* start, TTiXmlEncoding encoding );

	TTiXmlNode*		parent;
	NodeType		type;

	TTiXmlNode*		firstChild;
	TTiXmlNode*		lastChild;

	TTiXml_STRING	value;

	TTiXmlNode*		prev;
	TTiXmlNode*		next;

private:
	TTiXmlNode( const TTiXmlNode& );				// not implemented.
	void operator=( const TTiXmlNode& base );	// not allowed.
};


/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not TTiXmlNodes, since they are not
		  part of the tinyXML document object model. There are other
		  suggested ways to look at this problem.
*/
class TTiXmlAttribute : public TTiXmlBase
{
	friend class TTiXmlAttributeSet;

public:
	/// Construct an empty attribute.
	TTiXmlAttribute() : TTiXmlBase()
	{
		document = 0;
		prev = next = 0;
	}

	#ifdef TTiXml_USE_STL
	/// std::string constructor.
	TTiXmlAttribute( const std::string& _name, const std::string& _value )
	{
		name = _name;
		value = _value;
		document = 0;
		prev = next = 0;
	}
	#endif

	/// Construct an attribute with a name and value.
	TTiXmlAttribute( const char * _name, const char * _value )
	{
		name = _name;
		value = _value;
		document = 0;
		prev = next = 0;
	}

	const char*		Name()  const		{ return name.c_str (); }		///< Return the name of this attribute.
	const char*		Value() const		{ return value.c_str (); }		///< Return the value of this attribute.
	int				IntValue() const;									///< Return the value of this attribute, converted to an integer.
	double			DoubleValue() const;								///< Return the value of this attribute, converted to a double.

	// Get the tinyxml string representation
	const TTiXml_STRING& NameTStr() const { return name; }

	/** QueryIntValue examines the value string. It is an alternative to the
		IntValue() method with richer error checking.
		If the value is an integer, it is stored in 'value' and 
		the call returns TTiXml_SUCCESS. If it is not
		an integer, it returns TTiXml_WRONG_TYPE.

		A specialized but useful call. Note that for success it returns 0,
		which is the opposite of almost all other TinyXml calls.
	*/
	int QueryIntValue( int* _value ) const;
	/// QueryDoubleValue examines the value string. See QueryIntValue().
	int QueryDoubleValue( double* _value ) const;

	void SetName( const char* _name )	{ name = _name; }				///< Set the name of this attribute.
	void SetValue( const char* _value )	{ value = _value; }				///< Set the value.

	void SetIntValue( int _value );										///< Set the value from an integer.
	void SetDoubleValue( double _value );								///< Set the value from a double.

    #ifdef TTiXml_USE_STL
	/// STL std::string form.
	void SetName( const std::string& _name )	{ name = _name; }	
	/// STL std::string form.	
	void SetValue( const std::string& _value )	{ value = _value; }
	#endif

	/// Get the next sibling attribute in the DOM. Returns null at end.
	const TTiXmlAttribute* Next() const;
	TTiXmlAttribute* Next();
	/// Get the previous sibling attribute in the DOM. Returns null at beginning.
	const TTiXmlAttribute* Previous() const;
	TTiXmlAttribute* Previous();

	bool operator==( const TTiXmlAttribute& rhs ) const { return rhs.name == name; }
	bool operator<( const TTiXmlAttribute& rhs )	 const { return name < rhs.name; }
	bool operator>( const TTiXmlAttribute& rhs )  const { return name > rhs.name; }

	/*	Attribute parsing starts: first letter of the name
						 returns: the next char after the value end quote
	*/
	virtual const char* Parse( const char* p, TTiXmlParsingData* data, TTiXmlEncoding encoding );

	// Prints this Attribute to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	virtual void StreamOut( TTiXml_OSTREAM * out ) const;
	// [internal use]
	// Set the document pointer so the attribute can report errors.
	void SetDocument( TTiXmlDocument* doc )	{ document = doc; }

private:
	TTiXmlAttribute( const TTiXmlAttribute& );				// not implemented.
	void operator=( const TTiXmlAttribute& base );	// not allowed.

	TTiXmlDocument*	document;	// A pointer back to a document, for error reporting.
	TTiXml_STRING name;
	TTiXml_STRING value;
	TTiXmlAttribute*	prev;
	TTiXmlAttribute*	next;
};


/*	A class used to manage a group of attributes.
	It is only used internally, both by the ELEMENT and the DECLARATION.
	
	The set can be changed transparent to the Element and Declaration
	classes that use it, but NOT transparent to the Attribute
	which has to implement a next() and previous() method. Which makes
	it a bit problematic and prevents the use of STL.

	This version is implemented with circular lists because:
		- I like circular lists
		- it demonstrates some independence from the (typical) doubly linked list.
*/
class TTiXmlAttributeSet
{
public:
	TTiXmlAttributeSet();
	~TTiXmlAttributeSet();

	void Add( TTiXmlAttribute* attribute );
	void Remove( TTiXmlAttribute* attribute );

	const TTiXmlAttribute* First()	const	{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
	TTiXmlAttribute* First()					{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
	const TTiXmlAttribute* Last() const		{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }
	TTiXmlAttribute* Last()					{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }

	const TTiXmlAttribute*	Find( const TTiXml_STRING& name ) const;
	TTiXmlAttribute*	Find( const TTiXml_STRING& name );

private:
	//*ME:	Because of hidden/disabled copy-construktor in TTiXmlAttribute (sentinel-element),
	//*ME:	this class must be also use a hidden/disabled copy-constructor !!!
	TTiXmlAttributeSet( const TTiXmlAttributeSet& );	// not allowed
	void operator=( const TTiXmlAttributeSet& );	// not allowed (as TTiXmlAttribute)

	TTiXmlAttribute sentinel;
};


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class TTiXmlElement : public TTiXmlNode
{
public:
	/// Construct an element.
	TTiXmlElement (const char * in_value);

	#ifdef TTiXml_USE_STL
	/// std::string constructor.
	TTiXmlElement( const std::string& _value );
	#endif

	TTiXmlElement( const TTiXmlElement& );

	void operator=( const TTiXmlElement& base );

	virtual ~TTiXmlElement();

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
	*/
	const char* Attribute( const char* name ) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an integer,
		the integer value will be put in the return 'i', if 'i'
		is non-null.
	*/
	const char* Attribute( const char* name, int* i ) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an double,
		the double value will be put in the return 'd', if 'd'
		is non-null.
	*/
	const char* Attribute( const char* name, double* d ) const;

	/** QueryIntAttribute examines the attribute - it is an alternative to the
		Attribute() method with richer error checking.
		If the attribute is an integer, it is stored in 'value' and 
		the call returns TTiXml_SUCCESS. If it is not
		an integer, it returns TTiXml_WRONG_TYPE. If the attribute
		does not exist, then TTiXml_NO_ATTRIBUTE is returned.
	*/	
	int QueryIntAttribute( const char* name, int* _value ) const;
	/// QueryDoubleAttribute examines the attribute - see QueryIntAttribute().
	int QueryDoubleAttribute( const char* name, double* _value ) const;
	/// QueryFloatAttribute examines the attribute - see QueryIntAttribute().
	int QueryFloatAttribute( const char* name, float* _value ) const {
		double d;
		int result = QueryDoubleAttribute( name, &d );
		if ( result == TTiXml_SUCCESS ) {
			*_value = (float)d;
		}
		return result;
	}

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const char* name, const char * _value );

    #ifdef TTiXml_USE_STL
	const char* Attribute( const std::string& name ) const				{ return Attribute( name.c_str() ); }
	const char* Attribute( const std::string& name, int* i ) const		{ return Attribute( name.c_str(), i ); }
	const char* Attribute( const std::string& name, double* d ) const	{ return Attribute( name.c_str(), d ); }
	int QueryIntAttribute( const std::string& name, int* _value ) const	{ return QueryIntAttribute( name.c_str(), _value ); }
	int QueryDoubleAttribute( const std::string& name, double* _value ) const { return QueryDoubleAttribute( name.c_str(), _value ); }

	/// STL std::string form.
	void SetAttribute( const std::string& name, const std::string& _value );
	///< STL std::string form.
	void SetAttribute( const std::string& name, int _value );
	#endif

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const char * name, int value );

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetDoubleAttribute( const char * name, double value );

	/** Deletes an attribute with the given name.
	*/
	void RemoveAttribute( const char * name );
    #ifdef TTiXml_USE_STL
	void RemoveAttribute( const std::string& name )	{	RemoveAttribute (name.c_str ());	}	///< STL std::string form.
	#endif

	const TTiXmlAttribute* FirstAttribute() const	{ return attributeSet.First(); }		///< Access the first attribute in this element.
	TTiXmlAttribute* FirstAttribute() 				{ return attributeSet.First(); }
	const TTiXmlAttribute* LastAttribute()	const 	{ return attributeSet.Last(); }		///< Access the last attribute in this element.
	TTiXmlAttribute* LastAttribute()					{ return attributeSet.Last(); }

	/** Convenience function for easy access to the text inside an element. Although easy
		and concise, GetText() is limited compared to getting the TTiXmlText child
		and accessing it directly.
	
		If the first child of 'this' is a TTiXmlText, the GetText()
		returns the character string of the Text node, else null is returned.

		This is a convenient method for getting the text of simple contained text:
		@verbatim
		<foo>This is text</foo>
		const char* str = fooElement->GetText();
		@endverbatim

		'str' will be a pointer to "This is text". 
		
		Note that this function can be misleading. If the element foo was created from
		this XML:
		@verbatim
		<foo><b>This is text</b></foo> 
		@endverbatim

		then the value of str would be null. The first child node isn't a text node, it is
		another element. From this XML:
		@verbatim
		<foo>This is <b>text</b></foo> 
		@endverbatim
		GetText() will return "This is ".

		WARNING: GetText() accesses a child node - don't become confused with the 
				 similarly named TTiXmlHandle::Text() and TTiXmlNode::ToText() which are 
				 safe type casts on the referenced node.
	*/
	const char* GetText() const;

	/// Creates a new Element and returns it - the returned element is a copy.
	virtual TTiXmlNode* Clone() const;
	// Print the Element to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	/*	Attribtue parsing starts: next char past '<'
						 returns: next char past '>'
	*/
	virtual const char* Parse( const char* p, TTiXmlParsingData* data, TTiXmlEncoding encoding );

	virtual const TTiXmlElement*     ToElement()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TTiXmlElement*           ToElement()	          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	
protected:

	void CopyTo( TTiXmlElement* target ) const;
	void ClearThis();	// like clear, but initializes 'this' object as well

	// Used to be public [internal use]
	#ifdef TTiXml_USE_STL
	    virtual void StreamIn( TTiXml_ISTREAM * in, TTiXml_STRING * tag );
	#endif
	virtual void StreamOut( TTiXml_OSTREAM * out ) const;

	/*	[internal use]
		Reads the "value" of the element -- another element, or text.
		This should terminate with the current end tag.
	*/
	const char* ReadValue( const char* in, TTiXmlParsingData* prevData, TTiXmlEncoding encoding );

private:

	TTiXmlAttributeSet attributeSet;
};


/**	An XML comment.
*/
class TTiXmlComment : public TTiXmlNode
{
public:
	/// Constructs an empty comment.
	TTiXmlComment() : TTiXmlNode( TTiXmlNode::COMMENT ) {}
	TTiXmlComment( const TTiXmlComment& );
	void operator=( const TTiXmlComment& base );

	virtual ~TTiXmlComment()	{}

	/// Returns a copy of this Comment.
	virtual TTiXmlNode* Clone() const;
	/// Write this Comment to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	/*	Attribtue parsing starts: at the ! of the !--
						 returns: next char past '>'
	*/
	virtual const char* Parse( const char* p, TTiXmlParsingData* data, TTiXmlEncoding encoding );

	virtual const TTiXmlComment*  ToComment() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TTiXmlComment*  ToComment() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

protected:
	void CopyTo( TTiXmlComment* target ) const;

	// used to be public
	#ifdef TTiXml_USE_STL
	    virtual void StreamIn( TTiXml_ISTREAM * in, TTiXml_STRING * tag );
	#endif
	virtual void StreamOut( TTiXml_OSTREAM * out ) const;

private:

};


/** XML text. A text node can have 2 ways to output the next. "normal" output 
	and CDATA. It will default to the mode it was parsed from the XML file and
	you generally want to leave it alone, but you can change the output mode with 
	SetCDATA() and query it with CDATA().
*/
class TTiXmlText : public TTiXmlNode
{
	friend class TTiXmlElement;
public:
	/** Constructor for text element. By default, it is treated as 
		normal, encoded text. If you want it be output as a CDATA text
		element, set the parameter _cdata to 'true'
	*/
	TTiXmlText (const char * initValue ) : TTiXmlNode (TTiXmlNode::TEXT)
	{
		SetValue( initValue );
		cdata = false;
	}
	virtual ~TTiXmlText() {}

	#ifdef TTiXml_USE_STL
	/// Constructor.
	TTiXmlText( const std::string& initValue ) : TTiXmlNode (TTiXmlNode::TEXT)
	{
		SetValue( initValue );
		cdata = false;
	}
	#endif

	TTiXmlText( const TTiXmlText& copy ) : TTiXmlNode( TTiXmlNode::TEXT )	{ copy.CopyTo( this ); }
	void operator=( const TTiXmlText& base )							 	{ base.CopyTo( this ); }

	/// Write this text object to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	/// Queries whether this represents text using a CDATA section.
	bool CDATA()					{ return cdata; }
	/// Turns on or off a CDATA representation of text.
	void SetCDATA( bool _cdata )	{ cdata = _cdata; }

	virtual const char* Parse( const char* p, TTiXmlParsingData* data, TTiXmlEncoding encoding );

	virtual const TTiXmlText* ToText() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TTiXmlText*       ToText()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

protected :
	///  [internal use] Creates a new Element and returns it.
	virtual TTiXmlNode* Clone() const;
	void CopyTo( TTiXmlText* target ) const;

	virtual void StreamOut ( TTiXml_OSTREAM * out ) const;
	bool Blank() const;	// returns true if all white space and new lines
	// [internal use]
	#ifdef TTiXml_USE_STL
	    virtual void StreamIn( TTiXml_ISTREAM * in, TTiXml_STRING * tag );
	#endif

private:
	bool cdata;			// true if this should be input and output as a CDATA style text element
};


/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TinyXml will happily read or write files without a declaration,
	however. There are 3 possible attributes to the declaration:
	version, encoding, and standalone.

	Note: In this version of the code, the attributes are
	handled as special cases, not generic attributes, simply
	because there can only be at most 3 and they are always the same.
*/
class TTiXmlDeclaration : public TTiXmlNode
{
public:
	/// Construct an empty declaration.
	TTiXmlDeclaration()   : TTiXmlNode( TTiXmlNode::DECLARATION ) {}

#ifdef TTiXml_USE_STL
	/// Constructor.
	TTiXmlDeclaration(	const std::string& _version,
						const std::string& _encoding,
						const std::string& _standalone );
#endif

	/// Construct.
	TTiXmlDeclaration(	const char* _version,
						const char* _encoding,
						const char* _standalone );

	TTiXmlDeclaration( const TTiXmlDeclaration& copy );
	void operator=( const TTiXmlDeclaration& copy );

	virtual ~TTiXmlDeclaration()	{}

	/// Version. Will return an empty string if none was found.
	const char *Version() const			{ return version.c_str (); }
	/// Encoding. Will return an empty string if none was found.
	const char *Encoding() const		{ return encoding.c_str (); }
	/// Is this a standalone document?
	const char *Standalone() const		{ return standalone.c_str (); }

	/// Creates a copy of this Declaration and returns it.
	virtual TTiXmlNode* Clone() const;
	/// Print this declaration to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	virtual const char* Parse( const char* p, TTiXmlParsingData* data, TTiXmlEncoding encoding );

	virtual const TTiXmlDeclaration* ToDeclaration() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TTiXmlDeclaration*       ToDeclaration()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

protected:
	void CopyTo( TTiXmlDeclaration* target ) const;
	// used to be public
	#ifdef TTiXml_USE_STL
	    virtual void StreamIn( TTiXml_ISTREAM * in, TTiXml_STRING * tag );
	#endif
	virtual void StreamOut ( TTiXml_OSTREAM * out) const;

private:

	TTiXml_STRING version;
	TTiXml_STRING encoding;
	TTiXml_STRING standalone;
};


/** Any tag that tinyXml doesn't recognize is saved as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.

	DTD tags get thrown into TTiXmlUnknowns.
*/
class TTiXmlUnknown : public TTiXmlNode
{
public:
	TTiXmlUnknown() : TTiXmlNode( TTiXmlNode::UNKNOWN )	{}
	virtual ~TTiXmlUnknown() {}

	TTiXmlUnknown( const TTiXmlUnknown& copy ) : TTiXmlNode( TTiXmlNode::UNKNOWN )		{ copy.CopyTo( this ); }
	void operator=( const TTiXmlUnknown& copy )										{ copy.CopyTo( this ); }

	/// Creates a copy of this Unknown and returns it.
	virtual TTiXmlNode* Clone() const;
	/// Print this Unknown to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	virtual const char* Parse( const char* p, TTiXmlParsingData* data, TTiXmlEncoding encoding );

	virtual const TTiXmlUnknown*     ToUnknown()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TTiXmlUnknown*           ToUnknown()	    { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

protected:
	void CopyTo( TTiXmlUnknown* target ) const;

	#ifdef TTiXml_USE_STL
	    virtual void StreamIn( TTiXml_ISTREAM * in, TTiXml_STRING * tag );
	#endif
	virtual void StreamOut ( TTiXml_OSTREAM * out ) const;

private:

};


/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class TTiXmlDocument : public TTiXmlNode
{
public:
	/// Create an empty document, that has no name.
	TTiXmlDocument();
	/// Create a document with a name. The name of the document is also the filename of the xml.
	TTiXmlDocument( const char * documentName );

	#ifdef TTiXml_USE_STL
	/// Constructor.
	TTiXmlDocument( const std::string& documentName );
	#endif

	TTiXmlDocument( const TTiXmlDocument& copy );
	void operator=( const TTiXmlDocument& copy );

	virtual ~TTiXmlDocument() {}

	/** Load a file using the current document value.
		Returns true if successful. Will delete any existing
		document data before loading.
	*/
	bool LoadFile( TTiXmlEncoding encoding = TTiXml_DEFAULT_ENCODING );
	/// Save a file using the current document value. Returns true if successful.
	bool SaveFile() const;
	/// Load a file using the given filename. Returns true if successful.
	bool LoadFile( const char * filename, TTiXmlEncoding encoding = TTiXml_DEFAULT_ENCODING );
	/// Save a file using the given filename. Returns true if successful.
	bool SaveFile( const char * filename ) const;
	/** Load a file using the given FILE*. Returns true if successful. Note that this method
		doesn't stream - the entire object pointed at by the FILE*
		will be interpreted as an XML file. TinyXML doesn't stream in XML from the current
		file location. Streaming may be added in the future.
	*/
	bool LoadFile( FILE*, TTiXmlEncoding encoding = TTiXml_DEFAULT_ENCODING );
	/// Save a file using the given FILE*. Returns true if successful.
	bool SaveFile( FILE* ) const;

	#ifdef TTiXml_USE_STL
	bool LoadFile( const std::string& filename, TTiXmlEncoding encoding = TTiXml_DEFAULT_ENCODING )			///< STL std::string version.
	{
		StringToBuffer f( filename );
		return ( f.buffer && LoadFile( f.buffer, encoding ));
	}
	bool SaveFile( const std::string& filename ) const		///< STL std::string version.
	{
		StringToBuffer f( filename );
		return ( f.buffer && SaveFile( f.buffer ));
	}
	#endif

	/** Parse the given null terminated block of xml data. Passing in an encoding to this
		method (either TTiXml_ENCODING_LEGACY or TTiXml_ENCODING_UTF8 will force TinyXml
		to use that encoding, regardless of what TinyXml might otherwise try to detect.
	*/
	virtual const char* Parse( const char* p, TTiXmlParsingData* data = 0, TTiXmlEncoding encoding = TTiXml_DEFAULT_ENCODING );

	/** Get the root element -- the only top level element -- of the document.
		In well formed XML, there should only be one. TinyXml is tolerant of
		multiple elements at the document level.
	*/
	const TTiXmlElement* RootElement() const		{ return FirstChildElement(); }
	TTiXmlElement* RootElement()					{ return FirstChildElement(); }

	/** If an error occurs, Error will be set to true. Also,
		- The ErrorId() will contain the integer identifier of the error (not generally useful)
		- The ErrorDesc() method will return the name of the error. (very useful)
		- The ErrorRow() and ErrorCol() will return the location of the error (if known)
	*/	
	bool Error() const						{ return error; }

	/// Contains a textual (english) description of the error if one occurs.
	const char * ErrorDesc() const	{ return errorDesc.c_str (); }

	/** Generally, you probably want the error string ( ErrorDesc() ). But if you
		prefer the ErrorId, this function will fetch it.
	*/
	int ErrorId()	const				{ return errorId; }

	/** Returns the location (if known) of the error. The first column is column 1, 
		and the first row is row 1. A value of 0 means the row and column wasn't applicable
		(memory errors, for example, have no row/column) or the parser lost the error. (An
		error in the error reporting, in that case.)

		@sa SetTabSize, Row, Column
	*/
	int ErrorRow()	{ return errorLocation.row+1; }
	int ErrorCol()	{ return errorLocation.col+1; }	///< The column where the error occured. See ErrorRow()

	/** SetTabSize() allows the error reporting functions (ErrorRow() and ErrorCol())
		to report the correct values for row and column. It does not change the output
		or input in any way.
		
		By calling this method, with a tab size
		greater than 0, the row and column of each node and attribute is stored
		when the file is loaded. Very useful for tracking the DOM back in to
		the source file.

		The tab size is required for calculating the location of nodes. If not
		set, the default of 4 is used. The tabsize is set per document. Setting
		the tabsize to 0 disables row/column tracking.

		Note that row and column tracking is not supported when using operator>>.

		The tab size needs to be enabled before the parse or load. Correct usage:
		@verbatim
		TTiXmlDocument doc;
		doc.SetTabSize( 8 );
		doc.Load( "myfile.xml" );
		@endverbatim

		@sa Row, Column
	*/
	void SetTabSize( int _tabsize )		{ tabsize = _tabsize; }

	int TabSize() const	{ return tabsize; }

	/** If you have handled the error, it can be reset with this call. The error
		state is automatically cleared if you Parse a new XML block.
	*/
	void ClearError()						{	error = false; 
												errorId = 0; 
												errorDesc = ""; 
												errorLocation.row = errorLocation.col = 0; 
												//errorLocation.last = 0; 
											}

	/** Dump the document to standard out. */
	void Print() const						{ Print( stdout, 0 ); }

	/// Print this Document to a FILE stream.
	virtual void Print( FILE* cfile, int depth = 0 ) const;
	// [internal use]
	void SetError( int err, const char* errorLocation, TTiXmlParsingData* prevData, TTiXmlEncoding encoding );

	virtual const TTiXmlDocument*    ToDocument()    const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TTiXmlDocument*          ToDocument()          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

protected :
	virtual void StreamOut ( TTiXml_OSTREAM * out) const;
	// [internal use]
	virtual TTiXmlNode* Clone() const;
	#ifdef TTiXml_USE_STL
	    virtual void StreamIn( TTiXml_ISTREAM * in, TTiXml_STRING * tag );
	#endif

private:
	void CopyTo( TTiXmlDocument* target ) const;

	bool error;
	int  errorId;
	TTiXml_STRING errorDesc;
	int tabsize;
	TTiXmlCursor errorLocation;
	bool useMicrosoftBOM;		// the UTF-8 BOM were found when read. Note this, and try to write.
};


/**
	A TTiXmlHandle is a class that wraps a node pointer with null checks; this is
	an incredibly useful thing. Note that TTiXmlHandle is not part of the TinyXml
	DOM structure. It is a separate utility class.

	Take an example:
	@verbatim
	<Document>
		<Element attributeA = "valueA">
			<Child attributeB = "value1" />
			<Child attributeB = "value2" />
		</Element>
	<Document>
	@endverbatim

	Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very 
	easy to write a *lot* of code that looks like:

	@verbatim
	TTiXmlElement* root = document.FirstChildElement( "Document" );
	if ( root )
	{
		TTiXmlElement* element = root->FirstChildElement( "Element" );
		if ( element )
		{
			TTiXmlElement* child = element->FirstChildElement( "Child" );
			if ( child )
			{
				TTiXmlElement* child2 = child->NextSiblingElement( "Child" );
				if ( child2 )
				{
					// Finally do something useful.
	@endverbatim

	And that doesn't even cover "else" cases. TTiXmlHandle addresses the verbosity
	of such code. A TTiXmlHandle checks for null	pointers so it is perfectly safe 
	and correct to use:

	@verbatim
	TTiXmlHandle docHandle( &document );
	TTiXmlElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).Element();
	if ( child2 )
	{
		// do something useful
	@endverbatim

	Which is MUCH more concise and useful.

	It is also safe to copy handles - internally they are nothing more than node pointers.
	@verbatim
	TTiXmlHandle handleCopy = handle;
	@endverbatim

	What they should not be used for is iteration:

	@verbatim
	int i=0; 
	while ( true )
	{
		TTiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).Element();
		if ( !child )
			break;
		// do something
		++i;
	}
	@endverbatim

	It seems reasonable, but it is in fact two embedded while loops. The Child method is 
	a linear walk to find the element, so this code would iterate much more than it needs 
	to. Instead, prefer:

	@verbatim
	TTiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).Element();

	for( child; child; child=child->NextSiblingElement() )
	{
		// do something
	}
	@endverbatim
*/
class TTiXmlHandle
{
public:
	/// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
	TTiXmlHandle( TTiXmlNode* _node )					{ this->node = _node; }
	/// Copy constructor
	TTiXmlHandle( const TTiXmlHandle& ref )			{ this->node = ref.node; }
	TTiXmlHandle operator=( const TTiXmlHandle& ref ) { this->node = ref.node; return *this; }

	/// Return a handle to the first child node.
	TTiXmlHandle FirstChild() const;
	/// Return a handle to the first child node with the given name.
	TTiXmlHandle FirstChild( const char * value ) const;
	/// Return a handle to the first child element.
	TTiXmlHandle FirstChildElement() const;
	/// Return a handle to the first child element with the given name.
	TTiXmlHandle FirstChildElement( const char * value ) const;

	/** Return a handle to the "index" child with the given name. 
		The first child is 0, the second 1, etc.
	*/
	TTiXmlHandle Child( const char* value, int index ) const;
	/** Return a handle to the "index" child. 
		The first child is 0, the second 1, etc.
	*/
	TTiXmlHandle Child( int index ) const;
	/** Return a handle to the "index" child element with the given name. 
		The first child element is 0, the second 1, etc. Note that only TTiXmlElements
		are indexed: other types are not counted.
	*/
	TTiXmlHandle ChildElement( const char* value, int index ) const;
	/** Return a handle to the "index" child element. 
		The first child element is 0, the second 1, etc. Note that only TTiXmlElements
		are indexed: other types are not counted.
	*/
	TTiXmlHandle ChildElement( int index ) const;

	#ifdef TTiXml_USE_STL
	TTiXmlHandle FirstChild( const std::string& _value ) const				{ return FirstChild( _value.c_str() ); }
	TTiXmlHandle FirstChildElement( const std::string& _value ) const		{ return FirstChildElement( _value.c_str() ); }

	TTiXmlHandle Child( const std::string& _value, int index ) const			{ return Child( _value.c_str(), index ); }
	TTiXmlHandle ChildElement( const std::string& _value, int index ) const	{ return ChildElement( _value.c_str(), index ); }
	#endif

	/// Return the handle as a TTiXmlNode. This may return null.
	TTiXmlNode* Node() const			{ return node; } 
	/// Return the handle as a TTiXmlElement. This may return null.
	TTiXmlElement* Element() const	{ return ( ( node && node->ToElement() ) ? node->ToElement() : 0 ); }
	/// Return the handle as a TTiXmlText. This may return null.
	TTiXmlText* Text() const			{ return ( ( node && node->ToText() ) ? node->ToText() : 0 ); }
	/// Return the handle as a TTiXmlUnknown. This may return null;
	TTiXmlUnknown* Unknown() const			{ return ( ( node && node->ToUnknown() ) ? node->ToUnknown() : 0 ); }

private:
	TTiXmlNode* node;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif

