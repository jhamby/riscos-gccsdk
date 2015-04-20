--- Source/JavaScriptCore/dfg/DFGByteCodeParser.cpp.orig	2015-02-17 04:57:11.000000000 +0000
+++ Source/JavaScriptCore/dfg/DFGByteCodeParser.cpp	2015-04-16 15:18:10.737589538 +0100
@@ -1331,7 +1331,7 @@
     m_currentIndex = 0;
     m_currentProfilingIndex = 0;
 
-    addToGraph(InlineStart, OpInfo(argumentPositionStart));
+    addToGraph(InlineStart, OpInfo((uint32_t)argumentPositionStart));
     if (callLinkStatus.isClosureCall()) {
         addToGraph(SetCallee, callTargetNode);
         addToGraph(SetMyScope, addToGraph(GetScope, callTargetNode));
@@ -1686,7 +1686,7 @@
     // an OSR standpoint) if GetByOffset also referenced the object we were loading
     // from, and if we could load eliminate a GetByOffset even if the butterfly
     // had changed. That would be a great success.
-    Node* getByOffset = addToGraph(GetByOffset, OpInfo(m_graph.m_storageAccessData.size()), OpInfo(prediction), propertyStorage);
+    Node* getByOffset = addToGraph(GetByOffset, OpInfo((uint32_t)m_graph.m_storageAccessData.size()), OpInfo(prediction), propertyStorage);
 
     StorageAccessData storageAccessData;
     storageAccessData.offset = indexRelativeToBase(offset);
@@ -1882,7 +1882,7 @@
             return true;
         }
 
-        Node* resolve = addToGraph(ResolveGlobal, OpInfo(m_graph.m_resolveGlobalData.size()), OpInfo(prediction));
+        Node* resolve = addToGraph(ResolveGlobal, OpInfo((uint32_t)m_graph.m_resolveGlobalData.size()), OpInfo(prediction));
         m_graph.m_resolveGlobalData.append(ResolveGlobalData());
         ResolveGlobalData& data = m_graph.m_resolveGlobalData.last();
         data.identifierNumber = identifier;
@@ -2604,7 +2604,7 @@
                     propertyStorage = base;
                 else
                     propertyStorage = addToGraph(GetButterfly, base);
-                addToGraph(PutByOffset, OpInfo(m_graph.m_storageAccessData.size()), propertyStorage, base, value);
+                addToGraph(PutByOffset, OpInfo((uint32_t)m_graph.m_storageAccessData.size()), propertyStorage, base, value);
                 
                 StorageAccessData storageAccessData;
                 storageAccessData.offset = indexRelativeToBase(putByIdStatus.offset());
@@ -2667,7 +2667,7 @@
                 
                 addToGraph(
                     PutByOffset,
-                    OpInfo(m_graph.m_storageAccessData.size()),
+                    OpInfo((uint32_t)m_graph.m_storageAccessData.size()),
                     propertyStorage,
                     base,
                     value);
@@ -3142,7 +3142,7 @@
                 NEXT_OPCODE(op_resolve);
             }
 
-            Node* resolve = addToGraph(Resolve, OpInfo(m_graph.m_resolveOperationsData.size()), OpInfo(prediction));
+            Node* resolve = addToGraph(Resolve, OpInfo((uint32_t)m_graph.m_resolveOperationsData.size()), OpInfo(prediction));
             m_graph.m_resolveOperationsData.append(ResolveOperationData());
             ResolveOperationData& data = m_graph.m_resolveOperationsData.last();
             data.identifierNumber = identifier;
@@ -3209,7 +3209,7 @@
                     propertyStorage = baseNode;
                 else
                     propertyStorage = addToGraph(GetButterfly, baseNode);
-                addToGraph(PutByOffset, OpInfo(m_graph.m_storageAccessData.size()), propertyStorage, baseNode, get(value));
+                addToGraph(PutByOffset, OpInfo((uint32_t)m_graph.m_storageAccessData.size()), propertyStorage, baseNode, get(value));
 
                 StorageAccessData storageAccessData;
                 storageAccessData.offset = indexRelativeToBase(putToBase->m_offset);
@@ -3241,7 +3241,7 @@
                 NEXT_OPCODE(op_resolve_base);
             }
 
-            Node* resolve = addToGraph(currentInstruction[3].u.operand ? ResolveBaseStrictPut : ResolveBase, OpInfo(m_graph.m_resolveOperationsData.size()), OpInfo(prediction));
+            Node* resolve = addToGraph(currentInstruction[3].u.operand ? ResolveBaseStrictPut : ResolveBase, OpInfo((uint32_t)m_graph.m_resolveOperationsData.size()), OpInfo(prediction));
             m_graph.m_resolveOperationsData.append(ResolveOperationData());
             ResolveOperationData& data = m_graph.m_resolveOperationsData.last();
             data.identifierNumber = identifier;
