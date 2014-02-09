--- src/hb-open-type-private.hh.orig	2013-10-27 23:45:25.808585056 +0000
+++ src/hb-open-type-private.hh	2014-02-08 14:04:50.014160230 +0000
@@ -531,7 +531,7 @@
   inline bool operator == (const BEInt<Type, 2>& o) const { return hb_be_uint16_eq (v, o.v); }
   inline bool operator != (const BEInt<Type, 2>& o) const { return !(*this == o); }
   private: uint8_t v[2];
-};
+} PACKED;
 template <typename Type>
 struct BEInt<Type, 4>
 {
@@ -541,7 +541,7 @@
   inline bool operator == (const BEInt<Type, 4>& o) const { return hb_be_uint32_eq (v, o.v); }
   inline bool operator != (const BEInt<Type, 4>& o) const { return !(*this == o); }
   private: uint8_t v[4];
-};
+} PACKED;
 template <typename Type>
 struct BEInt<Type, 3>
 {
@@ -551,7 +551,7 @@
   inline bool operator == (const BEInt<Type, 3>& o) const { return hb_be_uint24_eq (v, o.v); }
   inline bool operator != (const BEInt<Type, 3>& o) const { return !(*this == o); }
   private: uint8_t v[3];
-};
+} PACKED;
 
 /* Integer types in big-endian order and no alignment requirement */
 template <typename Type, unsigned int Size>
@@ -572,7 +572,7 @@
   BEInt<Type, Size> v;
   public:
   DEFINE_SIZE_STATIC (Size);
-};
+} PACKED;
 
 typedef IntType<uint16_t, 2> USHORT;	/* 16-bit unsigned integer. */
 typedef IntType<int16_t,  2> SHORT;	/* 16-bit signed integer. */
@@ -599,7 +599,7 @@
   ULONG minor;
   public:
   DEFINE_SIZE_STATIC (8);
-};
+} PACKED;
 
 /* Array of four uint8s (length = 32 bits) used to identify a script, language
  * system, feature, or baseline */
@@ -610,7 +610,7 @@
   inline operator char* (void) { return reinterpret_cast<char *> (&this->v); }
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 DEFINE_NULL_DATA (Tag, "    ");
 
 /* Glyph index number, same as uint16 (length = 16 bits) */
@@ -619,7 +619,7 @@
 /* Script/language-system/feature index */
 struct Index : USHORT {
   static const unsigned int NOT_FOUND_INDEX = 0xFFFF;
-};
+} PACKED;
 DEFINE_NULL_DATA (Index, "\xff\xff");
 
 /* Offset to a table, same as uint16 (length = 16 bits), Null offset = 0x0000 */
@@ -628,7 +628,7 @@
   inline bool is_null (void) const { return 0 == *this; }
   public:
   DEFINE_SIZE_STATIC (2);
-};
+} PACKED;
 
 /* LongOffset to a table, same as uint32 (length = 32 bits), Null offset = 0x00000000 */
 struct LongOffset : ULONG
@@ -636,7 +636,7 @@
   inline bool is_null (void) const { return 0 == *this; }
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 
 /* CheckSum */
@@ -659,7 +659,7 @@
 
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 
 /*
@@ -679,7 +679,7 @@
   USHORT minor;
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 
 
@@ -738,17 +738,17 @@
     }
     return false;
   }
-};
+} PACKED;
 template <typename Base, typename OffsetType, typename Type>
 inline const Type& operator + (const Base &base, const GenericOffsetTo<OffsetType, Type> &offset) { return offset (base); }
 template <typename Base, typename OffsetType, typename Type>
 inline Type& operator + (Base &base, GenericOffsetTo<OffsetType, Type> &offset) { return offset (base); }
 
 template <typename Type>
-struct OffsetTo : GenericOffsetTo<Offset, Type> {};
+struct OffsetTo : GenericOffsetTo<Offset, Type> {} PACKED;
 
 template <typename Type>
-struct LongOffsetTo : GenericOffsetTo<LongOffset, Type> {};
+struct LongOffsetTo : GenericOffsetTo<LongOffset, Type> {} PACKED;
 
 
 /*
@@ -850,27 +850,27 @@
   Type array[VAR];
   public:
   DEFINE_SIZE_ARRAY (sizeof (LenType), array);
-};
+} PACKED;
 
 /* An array with a USHORT number of elements. */
 template <typename Type>
-struct ArrayOf : GenericArrayOf<USHORT, Type> {};
+struct ArrayOf : GenericArrayOf<USHORT, Type> {} PACKED;
 
 /* An array with a ULONG number of elements. */
 template <typename Type>
-struct LongArrayOf : GenericArrayOf<ULONG, Type> {};
+struct LongArrayOf : GenericArrayOf<ULONG, Type> {} PACKED;
 
 /* Array of Offset's */
 template <typename Type>
-struct OffsetArrayOf : ArrayOf<OffsetTo<Type> > {};
+struct OffsetArrayOf : ArrayOf<OffsetTo<Type> > {} PACKED;
 
 /* Array of LongOffset's */
 template <typename Type>
-struct LongOffsetArrayOf : ArrayOf<LongOffsetTo<Type> > {};
+struct LongOffsetArrayOf : ArrayOf<LongOffsetTo<Type> > {} PACKED;
 
 /* LongArray of LongOffset's */
 template <typename Type>
-struct LongOffsetLongArrayOf : LongArrayOf<LongOffsetTo<Type> > {};
+struct LongOffsetLongArrayOf : LongArrayOf<LongOffsetTo<Type> > {} PACKED;
 
 /* Array of offsets relative to the beginning of the array itself. */
 template <typename Type>
@@ -891,7 +891,7 @@
     TRACE_SANITIZE (this);
     return TRACE_RETURN (OffsetArrayOf<Type>::sanitize (c, this, user_data));
   }
-};
+} PACKED;
 
 
 /* An array with a USHORT number of elements,
@@ -947,7 +947,7 @@
   Type array[VAR];
   public:
   DEFINE_SIZE_ARRAY (sizeof (USHORT), array);
-};
+} PACKED;
 
 
 /* An array with sorted elements.  Supports binary searching. */
@@ -972,7 +972,7 @@
     }
     return -1;
   }
-};
+} PACKED;
 
 
 } /* namespace OT */
