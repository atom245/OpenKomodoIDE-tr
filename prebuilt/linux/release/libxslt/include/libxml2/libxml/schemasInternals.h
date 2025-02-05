/*
 * Summary: internal interfaces for XML Schemas
 * Description: internal interfaces for the XML Schemas handling
 *              and schema validity checking
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */


#ifndef __XML_SCHEMA_INTERNALS_H__
#define __XML_SCHEMA_INTERNALS_H__

#include <libxml/xmlversion.h>

#ifdef LIBXML_SCHEMAS_ENABLED

#include <libxml/xmlregexp.h>
#include <libxml/hash.h>
#include <libxml/dict.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * XML Schemas defines multiple type of types.
 */
typedef enum {
    XML_SCHEMA_TYPE_BASIC = 1,
    XML_SCHEMA_TYPE_ANY,
    XML_SCHEMA_TYPE_FACET,
    XML_SCHEMA_TYPE_SIMPLE,
    XML_SCHEMA_TYPE_COMPLEX,
    XML_SCHEMA_TYPE_SEQUENCE,
    XML_SCHEMA_TYPE_CHOICE,
    XML_SCHEMA_TYPE_ALL,
    XML_SCHEMA_TYPE_SIMPLE_CONTENT,
    XML_SCHEMA_TYPE_COMPLEX_CONTENT,
    XML_SCHEMA_TYPE_UR,
    XML_SCHEMA_TYPE_RESTRICTION,
    XML_SCHEMA_TYPE_EXTENSION,
    XML_SCHEMA_TYPE_ELEMENT,
    XML_SCHEMA_TYPE_ATTRIBUTE,
    XML_SCHEMA_TYPE_ATTRIBUTEGROUP,
    XML_SCHEMA_TYPE_GROUP,
    XML_SCHEMA_TYPE_NOTATION,
    XML_SCHEMA_TYPE_LIST,
    XML_SCHEMA_TYPE_UNION,
    XML_SCHEMA_FACET_MININCLUSIVE = 1000,
    XML_SCHEMA_FACET_MINEXCLUSIVE,
    XML_SCHEMA_FACET_MAXINCLUSIVE,
    XML_SCHEMA_FACET_MAXEXCLUSIVE,
    XML_SCHEMA_FACET_TOTALDIGITS,
    XML_SCHEMA_FACET_FRACTIONDIGITS,
    XML_SCHEMA_FACET_PATTERN,
    XML_SCHEMA_FACET_ENUMERATION,
    XML_SCHEMA_FACET_WHITESPACE,
    XML_SCHEMA_FACET_LENGTH,
    XML_SCHEMA_FACET_MAXLENGTH,
    XML_SCHEMA_FACET_MINLENGTH
} xmlSchemaTypeType;

typedef enum {
    XML_SCHEMA_CONTENT_UNKNOWN = 0,
    XML_SCHEMA_CONTENT_EMPTY = 1,
    XML_SCHEMA_CONTENT_ELEMENTS,
    XML_SCHEMA_CONTENT_MIXED,
    XML_SCHEMA_CONTENT_SIMPLE,
    XML_SCHEMA_CONTENT_MIXED_OR_ELEMENTS,
    XML_SCHEMA_CONTENT_BASIC,
    XML_SCHEMA_CONTENT_ANY
} xmlSchemaContentType;

typedef struct _xmlSchemaVal xmlSchemaVal;
typedef xmlSchemaVal *xmlSchemaValPtr;

typedef struct _xmlSchemaType xmlSchemaType;
typedef xmlSchemaType *xmlSchemaTypePtr;

typedef struct _xmlSchemaFacet xmlSchemaFacet;
typedef xmlSchemaFacet *xmlSchemaFacetPtr;

/**
 * Annotation
 */
typedef struct _xmlSchemaAnnot xmlSchemaAnnot;
typedef xmlSchemaAnnot *xmlSchemaAnnotPtr;
struct _xmlSchemaAnnot {
    struct _xmlSchemaAnnot *next;
    xmlNodePtr content;         /* the annotation */
};

/**
 * XML_SCHEMAS_ANYATTR_SKIP:
 *
 * Skip unknown attribute from validation
 */
#define XML_SCHEMAS_ANYATTR_SKIP	1
/**
 * XML_SCHEMAS_ANYATTR_LAX:
 *
 * Ignore validation non definition on attributes
 */
#define XML_SCHEMAS_ANYATTR_LAX		2
/**
 * XML_SCHEMAS_ANYATTR_STRICT:
 *
 * Apply strict validation rules on attributes
 */
#define XML_SCHEMAS_ANYATTR_STRICT	3

/**
 * XML_SCHEMAS_ATTR_NSDEFAULT:
 *
 * allow elements in no namespace
 */
#define XML_SCHEMAS_ATTR_NSDEFAULT	1 << 7

/**
 * xmlSchemaAttribute:
 * An attribute definition.
 */

typedef struct _xmlSchemaAttribute xmlSchemaAttribute;
typedef xmlSchemaAttribute *xmlSchemaAttributePtr;
struct _xmlSchemaAttribute {
    xmlSchemaTypeType type;	/* The kind of type */
    struct _xmlSchemaAttribute *next;/* the next attribute if in a group ... */
    const xmlChar *name;
    const xmlChar *id;
    const xmlChar *ref;
    const xmlChar *refNs;
    const xmlChar *typeName;
    const xmlChar *typeNs;
    xmlSchemaAnnotPtr annot;

    xmlSchemaTypePtr base;
    int occurs;
    const xmlChar *defValue;
    xmlSchemaTypePtr subtypes;
    xmlNodePtr node;
    const xmlChar *targetNamespace;
    int flags;
};

/**
 * An attribute group definition.
 *
 * xmlSchemaAttribute and xmlSchemaAttributeGroup start of structures
 * must be kept similar
 */
typedef struct _xmlSchemaAttributeGroup xmlSchemaAttributeGroup;
typedef xmlSchemaAttributeGroup *xmlSchemaAttributeGroupPtr;
struct _xmlSchemaAttributeGroup {
    xmlSchemaTypeType type;	/* The kind of type */
    struct _xmlSchemaAttribute *next;/* the next attribute if in a group ... */
    const xmlChar *name;
    const xmlChar *id;
    const xmlChar *ref;
    const xmlChar *refNs;
    xmlSchemaAnnotPtr annot;

    xmlSchemaAttributePtr attributes;
    xmlNodePtr node;
};


/**
 * XML_SCHEMAS_TYPE_MIXED:
 *
 * the element content type is mixed
 */
#define XML_SCHEMAS_TYPE_MIXED		1 << 0

/**
 * _xmlSchemaType:
 *
 * Schemas type definition.
 */
struct _xmlSchemaType {
    xmlSchemaTypeType type;	/* The kind of type */
    struct _xmlSchemaType *next;/* the next type if in a sequence ... */
    const xmlChar *name;
    const xmlChar *id;
    const xmlChar *ref;
    const xmlChar *refNs;
    xmlSchemaAnnotPtr annot;
    xmlSchemaTypePtr subtypes;
    xmlSchemaAttributePtr attributes;
    xmlNodePtr node;
    int minOccurs;
    int maxOccurs;

    int flags;
    xmlSchemaContentType contentType;
    const xmlChar *base;
    const xmlChar *baseNs;
    xmlSchemaTypePtr baseType;
    xmlSchemaFacetPtr facets;
    struct _xmlSchemaType *redef;/* possible redefinitions for the type */
};

/*
 * xmlSchemaElement:
 * An element definition.
 *
 * xmlSchemaType, xmlSchemaFacet and xmlSchemaElement start of
 * structures must be kept similar
 */
/**
 * XML_SCHEMAS_ELEM_NILLABLE:
 *
 * the element is nillable
 */
#define XML_SCHEMAS_ELEM_NILLABLE	1 << 0
/**
 * XML_SCHEMAS_ELEM_GLOBAL:
 *
 * the element is global
 */
#define XML_SCHEMAS_ELEM_GLOBAL		1 << 1
/**
 * XML_SCHEMAS_ELEM_DEFAULT:
 *
 * the element has a default value
 */
#define XML_SCHEMAS_ELEM_DEFAULT	1 << 2
/**
 * XML_SCHEMAS_ELEM_FIXED:
 *
 * the element has a fixed value
 */
#define XML_SCHEMAS_ELEM_FIXED		1 << 3
/**
 * XML_SCHEMAS_ELEM_ABSTRACT:
 *
 * the element is abstract
 */
#define XML_SCHEMAS_ELEM_ABSTRACT	1 << 4
/**
 * XML_SCHEMAS_ELEM_TOPLEVEL:
 *
 * the element is top level
 */
#define XML_SCHEMAS_ELEM_TOPLEVEL	1 << 5
/**
 * XML_SCHEMAS_ELEM_REF:
 *
 * the element is a reference to a type
 */
#define XML_SCHEMAS_ELEM_REF		1 << 6
/**
 * XML_SCHEMAS_ELEM_NSDEFAULT:
 *
 * allow elements in no namespace
 */
#define XML_SCHEMAS_ELEM_NSDEFAULT	1 << 7

typedef struct _xmlSchemaElement xmlSchemaElement;
typedef xmlSchemaElement *xmlSchemaElementPtr;
struct _xmlSchemaElement {
    xmlSchemaTypeType type;	/* The kind of type */
    struct _xmlSchemaType *next;/* the next type if in a sequence ... */
    const xmlChar *name;
    const xmlChar *id;
    const xmlChar *ref;
    const xmlChar *refNs;
    xmlSchemaAnnotPtr annot;
    xmlSchemaTypePtr subtypes;
    xmlSchemaAttributePtr attributes;
    xmlNodePtr node;
    int minOccurs;
    int maxOccurs;

    int flags;
    const xmlChar *targetNamespace;
    const xmlChar *namedType;
    const xmlChar *namedTypeNs;
    const xmlChar *substGroup;
    const xmlChar *substGroupNs;
    const xmlChar *scope;
    const xmlChar *value;
    struct _xmlSchemaElement *refDecl;
    xmlRegexpPtr contModel;
    xmlSchemaContentType contentType;
};

/*
 * XML_SCHEMAS_FACET_UNKNOWN:
 *
 * unknown facet handling
 */
#define XML_SCHEMAS_FACET_UNKNOWN	0
/*
 * XML_SCHEMAS_FACET_PRESERVE:
 *
 * preserve the type of the facet
 */
#define XML_SCHEMAS_FACET_PRESERVE	1
/*
 * XML_SCHEMAS_FACET_REPLACE:
 *
 * replace the type of the facet
 */
#define XML_SCHEMAS_FACET_REPLACE	2
/*
 * XML_SCHEMAS_FACET_COLLAPSE:
 *
 * collapse the types of the facet
 */
#define XML_SCHEMAS_FACET_COLLAPSE	3

/**
 * A facet definition.
 */
struct _xmlSchemaFacet {
    xmlSchemaTypeType type;	/* The kind of type */
    struct _xmlSchemaFacet *next;/* the next type if in a sequence ... */
    const xmlChar *value;
    const xmlChar *id;
    xmlSchemaAnnotPtr annot;
    xmlNodePtr node;
    int fixed;
    int whitespace;
    xmlSchemaValPtr val;
    xmlRegexpPtr    regexp;
};

/**
 * A notation definition.
 */
typedef struct _xmlSchemaNotation xmlSchemaNotation;
typedef xmlSchemaNotation *xmlSchemaNotationPtr;
struct _xmlSchemaNotation {
    xmlSchemaTypeType type;	/* The kind of type */
    const xmlChar *name;
    xmlSchemaAnnotPtr annot;
    const xmlChar *identifier;
};

/**
 * XML_SCHEMAS_QUALIF_ELEM:
 *
 * the shemas requires qualified elements
 */
#define XML_SCHEMAS_QUALIF_ELEM		1 << 0
/**
 * XML_SCHEMAS_QUALIF_ATTR:
 *
 * the shemas requires qualified attributes
 */
#define XML_SCHEMAS_QUALIF_ATTR		1 << 1
/**
 * _xmlSchema:
 *
 * A Schemas definition
 */
struct _xmlSchema {
    const xmlChar *name;        /* schema name */
    const xmlChar *targetNamespace;     /* the target namespace */
    const xmlChar *version;
    const xmlChar *id;
    xmlDocPtr doc;
    xmlSchemaAnnotPtr annot;
    int flags;

    xmlHashTablePtr typeDecl;
    xmlHashTablePtr attrDecl;
    xmlHashTablePtr attrgrpDecl;
    xmlHashTablePtr elemDecl;
    xmlHashTablePtr notaDecl;

    xmlHashTablePtr schemasImports;

    void *_private;	/* unused by the library for users or bindings */
    xmlHashTablePtr groupDecl;
    xmlDictPtr      dict;
    void *includes;     /* the includes, this is opaque for now */
};

XMLPUBFUN void XMLCALL 	xmlSchemaFreeType	(xmlSchemaTypePtr type);

#ifdef __cplusplus
}
#endif

#endif /* LIBXML_SCHEMAS_ENABLED */
#endif /* __XML_SCHEMA_INTERNALS_H__ */


