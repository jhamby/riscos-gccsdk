--- src/hb-open-type-private.hh.orig	2014-07-25 16:19:44.520518030 +0100
+++ src/hb-open-type-private.hh	2018-11-25 15:08:23.710611040 +0000
@@ -538,7 +538,7 @@
   inline bool operator == (const BEInt<Type, 2>& o) const { return hb_be_uint16_eq (v, o.v); }
   inline bool operator != (const BEInt<Type, 2>& o) const { return !(*this == o); }
   private: uint8_t v[2];
-};
+} PACKED;
 template <typename Type>
 struct BEInt<Type, 4>
 {
@@ -548,7 +548,7 @@
   inline bool operator == (const BEInt<Type, 4>& o) const { return hb_be_uint32_eq (v, o.v); }
   inline bool operator != (const BEInt<Type, 4>& o) const { return !(*this == o); }
   private: uint8_t v[4];
-};
+} PACKED;
 template <typename Type>
 struct BEInt<Type, 3>
 {
@@ -558,7 +558,7 @@
   inline bool operator == (const BEInt<Type, 3>& o) const { return hb_be_uint24_eq (v, o.v); }
   inline bool operator != (const BEInt<Type, 3>& o) const { return !(*this == o); }
   private: uint8_t v[3];
-};
+} PACKED;
 
 /* Integer types in big-endian order and no alignment requirement */
 template <typename Type, unsigned int Size>
@@ -579,7 +579,7 @@
   BEInt<Type, Size> v;
   public:
   DEFINE_SIZE_STATIC (Size);
-};
+} PACKED;
 
 typedef		uint8_t	     BYTE;	/* 8-bit unsigned integer. */
 typedef IntType<uint16_t, 2> USHORT;	/* 16-bit unsigned integer. */
@@ -607,7 +607,7 @@
   ULONG minor;
   public:
   DEFINE_SIZE_STATIC (8);
-};
+} PACKED;
 
 /* Array of four uint8s (length = 32 bits) used to identify a script, language
  * system, feature, or baseline */
@@ -618,7 +618,7 @@
   inline operator char* (void) { return reinterpret_cast<char *> (&this->v); }
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 DEFINE_NULL_DATA (Tag, "    ");
 
 /* Glyph index number, same as uint16 (length = 16 bits) */
@@ -627,7 +627,7 @@
 /* Script/language-system/feature index */
 struct Index : USHORT {
   static const unsigned int NOT_FOUND_INDEX = 0xFFFFu;
-};
+} PACKED;
 DEFINE_NULL_DATA (Index, "\xff\xff");
 
 /* Offset, Null offset = 0 */
@@ -637,7 +637,7 @@
   inline bool is_null (void) const { return 0 == *this; }
   public:
   DEFINE_SIZE_STATIC (sizeof(Type));
-};
+} PACKED;
 
 
 /* CheckSum */
@@ -660,7 +660,7 @@
 
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 
 /*
@@ -680,7 +680,7 @@
   USHORT minor;
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 
 
@@ -871,7 +871,7 @@
     TRACE_SANITIZE (this);
     return TRACE_RETURN (OffsetArrayOf<Type>::sanitize (c, this, user_data));
   }
-};
+} PACKED;
 
 
 /* An array starting at second element. */
@@ -951,7 +951,7 @@
     }
     return -1;
   }
-};
+} PACKED;
 
 
 } /* namespace OT */
