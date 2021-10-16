#include "src/objects/fixed-array-inl.h"
#include "src/objects/free-space.h"
#include "src/objects/js-regexp-string-iterator.h"
#include "src/objects/js-weak-refs.h"
#include "src/objects/ordered-hash-table.h"
#include "src/objects/property-descriptor-object.h"
#include "src/objects/stack-frame-info.h"
#include "src/objects/swiss-name-dictionary.h"
#include "src/objects/synthetic-module.h"
#include "src/objects/template-objects.h"
#include "src/objects/torque-defined-classes.h"
#ifdef V8_INTL_SUPPORT
#include "src/objects/js-break-iterator.h"
#include "src/objects/js-collator.h"
#include "src/objects/js-date-time-format.h"
#include "src/objects/js-display-names.h"
#include "src/objects/js-list-format.h"
#include "src/objects/js-locale.h"
#include "src/objects/js-number-format.h"
#include "src/objects/js-plural-rules.h"
#include "src/objects/js-relative-time-format.h"
#include "src/objects/js-segment-iterator.h"
#include "src/objects/js-segmenter.h"
#include "src/objects/js-segments.h"
#endif  // V8_INTL_SUPPORT
#include "torque-generated/exported-macros-assembler.h"
#include "torque-generated/src/builtins/aggregate-error-tq-csa.h"
#include "torque-generated/src/builtins/array-at-tq-csa.h"
#include "torque-generated/src/builtins/array-copywithin-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-filter-tq-csa.h"
#include "torque-generated/src/builtins/array-find-tq-csa.h"
#include "torque-generated/src/builtins/array-findindex-tq-csa.h"
#include "torque-generated/src/builtins/array-foreach-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/array-isarray-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/array-lastindexof-tq-csa.h"
#include "torque-generated/src/builtins/array-map-tq-csa.h"
#include "torque-generated/src/builtins/array-of-tq-csa.h"
#include "torque-generated/src/builtins/array-reduce-right-tq-csa.h"
#include "torque-generated/src/builtins/array-reduce-tq-csa.h"
#include "torque-generated/src/builtins/array-reverse-tq-csa.h"
#include "torque-generated/src/builtins/array-shift-tq-csa.h"
#include "torque-generated/src/builtins/array-slice-tq-csa.h"
#include "torque-generated/src/builtins/array-some-tq-csa.h"
#include "torque-generated/src/builtins/array-splice-tq-csa.h"
#include "torque-generated/src/builtins/array-unshift-tq-csa.h"
#include "torque-generated/src/builtins/array-tq-csa.h"
#include "torque-generated/src/builtins/arraybuffer-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/boolean-tq-csa.h"
#include "torque-generated/src/builtins/builtins-bigint-tq-csa.h"
#include "torque-generated/src/builtins/builtins-string-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/collections-tq-csa.h"
#include "torque-generated/src/builtins/constructor-tq-csa.h"
#include "torque-generated/src/builtins/conversion-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/console-tq-csa.h"
#include "torque-generated/src/builtins/data-view-tq-csa.h"
#include "torque-generated/src/builtins/finalization-registry-tq-csa.h"
#include "torque-generated/src/builtins/frames-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/function-tq-csa.h"
#include "torque-generated/src/builtins/growable-fixed-array-tq-csa.h"
#include "torque-generated/src/builtins/ic-callable-tq-csa.h"
#include "torque-generated/src/builtins/ic-dynamic-check-maps-tq-csa.h"
#include "torque-generated/src/builtins/ic-tq-csa.h"
#include "torque-generated/src/builtins/internal-coverage-tq-csa.h"
#include "torque-generated/src/builtins/internal-tq-csa.h"
#include "torque-generated/src/builtins/iterator-tq-csa.h"
#include "torque-generated/src/builtins/math-tq-csa.h"
#include "torque-generated/src/builtins/number-tq-csa.h"
#include "torque-generated/src/builtins/object-fromentries-tq-csa.h"
#include "torque-generated/src/builtins/object-tq-csa.h"
#include "torque-generated/src/builtins/promise-abstract-operations-tq-csa.h"
#include "torque-generated/src/builtins/promise-all-tq-csa.h"
#include "torque-generated/src/builtins/promise-all-element-closure-tq-csa.h"
#include "torque-generated/src/builtins/promise-any-tq-csa.h"
#include "torque-generated/src/builtins/promise-constructor-tq-csa.h"
#include "torque-generated/src/builtins/promise-finally-tq-csa.h"
#include "torque-generated/src/builtins/promise-misc-tq-csa.h"
#include "torque-generated/src/builtins/promise-race-tq-csa.h"
#include "torque-generated/src/builtins/promise-reaction-job-tq-csa.h"
#include "torque-generated/src/builtins/promise-resolve-tq-csa.h"
#include "torque-generated/src/builtins/promise-then-tq-csa.h"
#include "torque-generated/src/builtins/promise-jobs-tq-csa.h"
#include "torque-generated/src/builtins/proxy-constructor-tq-csa.h"
#include "torque-generated/src/builtins/proxy-delete-property-tq-csa.h"
#include "torque-generated/src/builtins/proxy-get-property-tq-csa.h"
#include "torque-generated/src/builtins/proxy-get-prototype-of-tq-csa.h"
#include "torque-generated/src/builtins/proxy-has-property-tq-csa.h"
#include "torque-generated/src/builtins/proxy-is-extensible-tq-csa.h"
#include "torque-generated/src/builtins/proxy-prevent-extensions-tq-csa.h"
#include "torque-generated/src/builtins/proxy-revocable-tq-csa.h"
#include "torque-generated/src/builtins/proxy-revoke-tq-csa.h"
#include "torque-generated/src/builtins/proxy-set-property-tq-csa.h"
#include "torque-generated/src/builtins/proxy-set-prototype-of-tq-csa.h"
#include "torque-generated/src/builtins/proxy-tq-csa.h"
#include "torque-generated/src/builtins/reflect-tq-csa.h"
#include "torque-generated/src/builtins/regexp-exec-tq-csa.h"
#include "torque-generated/src/builtins/regexp-match-all-tq-csa.h"
#include "torque-generated/src/builtins/regexp-match-tq-csa.h"
#include "torque-generated/src/builtins/regexp-replace-tq-csa.h"
#include "torque-generated/src/builtins/regexp-search-tq-csa.h"
#include "torque-generated/src/builtins/regexp-source-tq-csa.h"
#include "torque-generated/src/builtins/regexp-split-tq-csa.h"
#include "torque-generated/src/builtins/regexp-test-tq-csa.h"
#include "torque-generated/src/builtins/regexp-tq-csa.h"
#include "torque-generated/src/builtins/string-at-tq-csa.h"
#include "torque-generated/src/builtins/string-endswith-tq-csa.h"
#include "torque-generated/src/builtins/string-html-tq-csa.h"
#include "torque-generated/src/builtins/string-includes-tq-csa.h"
#include "torque-generated/src/builtins/string-indexof-tq-csa.h"
#include "torque-generated/src/builtins/string-iterator-tq-csa.h"
#include "torque-generated/src/builtins/string-match-search-tq-csa.h"
#include "torque-generated/src/builtins/string-pad-tq-csa.h"
#include "torque-generated/src/builtins/string-repeat-tq-csa.h"
#include "torque-generated/src/builtins/string-replaceall-tq-csa.h"
#include "torque-generated/src/builtins/string-slice-tq-csa.h"
#include "torque-generated/src/builtins/string-startswith-tq-csa.h"
#include "torque-generated/src/builtins/string-substr-tq-csa.h"
#include "torque-generated/src/builtins/string-substring-tq-csa.h"
#include "torque-generated/src/builtins/string-trim-tq-csa.h"
#include "torque-generated/src/builtins/symbol-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-at-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-createtypedarray-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-every-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-entries-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-filter-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-find-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-findindex-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-foreach-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-from-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-keys-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-of-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-reduce-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-reduceright-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-set-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-slice-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-some-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-sort-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-subarray-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-values-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"
#include "torque-generated/src/builtins/weak-ref-tq-csa.h"
#include "torque-generated/src/ic/handler-configuration-tq-csa.h"
#include "torque-generated/src/objects/allocation-site-tq-csa.h"
#include "torque-generated/src/objects/api-callbacks-tq-csa.h"
#include "torque-generated/src/objects/arguments-tq-csa.h"
#include "torque-generated/src/objects/bigint-tq-csa.h"
#include "torque-generated/src/objects/cell-tq-csa.h"
#include "torque-generated/src/objects/code-tq-csa.h"
#include "torque-generated/src/objects/contexts-tq-csa.h"
#include "torque-generated/src/objects/data-handler-tq-csa.h"
#include "torque-generated/src/objects/debug-objects-tq-csa.h"
#include "torque-generated/src/objects/descriptor-array-tq-csa.h"
#include "torque-generated/src/objects/embedder-data-array-tq-csa.h"
#include "torque-generated/src/objects/feedback-cell-tq-csa.h"
#include "torque-generated/src/objects/feedback-vector-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/foreign-tq-csa.h"
#include "torque-generated/src/objects/free-space-tq-csa.h"
#include "torque-generated/src/objects/heap-number-tq-csa.h"
#include "torque-generated/src/objects/heap-object-tq-csa.h"
#include "torque-generated/src/objects/js-array-buffer-tq-csa.h"
#include "torque-generated/src/objects/js-array-tq-csa.h"
#include "torque-generated/src/objects/js-collection-iterator-tq-csa.h"
#include "torque-generated/src/objects/js-collection-tq-csa.h"
#include "torque-generated/src/objects/js-function-tq-csa.h"
#include "torque-generated/src/objects/js-generator-tq-csa.h"
#include "torque-generated/src/objects/js-objects-tq-csa.h"
#include "torque-generated/src/objects/js-promise-tq-csa.h"
#include "torque-generated/src/objects/js-proxy-tq-csa.h"
#include "torque-generated/src/objects/js-regexp-string-iterator-tq-csa.h"
#include "torque-generated/src/objects/js-regexp-tq-csa.h"
#include "torque-generated/src/objects/js-weak-refs-tq-csa.h"
#include "torque-generated/src/objects/literal-objects-tq-csa.h"
#include "torque-generated/src/objects/map-tq-csa.h"
#include "torque-generated/src/objects/megadom-handler-tq-csa.h"
#include "torque-generated/src/objects/microtask-tq-csa.h"
#include "torque-generated/src/objects/module-tq-csa.h"
#include "torque-generated/src/objects/name-tq-csa.h"
#include "torque-generated/src/objects/oddball-tq-csa.h"
#include "torque-generated/src/objects/ordered-hash-table-tq-csa.h"
#include "torque-generated/src/objects/primitive-heap-object-tq-csa.h"
#include "torque-generated/src/objects/promise-tq-csa.h"
#include "torque-generated/src/objects/property-array-tq-csa.h"
#include "torque-generated/src/objects/property-cell-tq-csa.h"
#include "torque-generated/src/objects/property-descriptor-object-tq-csa.h"
#include "torque-generated/src/objects/prototype-info-tq-csa.h"
#include "torque-generated/src/objects/regexp-match-info-tq-csa.h"
#include "torque-generated/src/objects/scope-info-tq-csa.h"
#include "torque-generated/src/objects/script-tq-csa.h"
#include "torque-generated/src/objects/shared-function-info-tq-csa.h"
#include "torque-generated/src/objects/source-text-module-tq-csa.h"
#include "torque-generated/src/objects/stack-frame-info-tq-csa.h"
#include "torque-generated/src/objects/string-tq-csa.h"
#include "torque-generated/src/objects/struct-tq-csa.h"
#include "torque-generated/src/objects/swiss-hash-table-helpers-tq-csa.h"
#include "torque-generated/src/objects/swiss-name-dictionary-tq-csa.h"
#include "torque-generated/src/objects/synthetic-module-tq-csa.h"
#include "torque-generated/src/objects/template-objects-tq-csa.h"
#include "torque-generated/src/objects/templates-tq-csa.h"
#include "torque-generated/src/objects/torque-defined-classes-tq-csa.h"
#include "torque-generated/test/torque/test-torque-tq-csa.h"
#include "torque-generated/third_party/v8/builtins/array-sort-tq-csa.h"
namespace v8 {
namespace internal {
void TorqueGeneratedExportedMacrosAssembler::EnsureArrayLengthWritable(TNode<Context> p_context, TNode<Map> p_map, compiler::CodeAssemblerLabel* label_Bailout) {
return EnsureArrayLengthWritable_0(state_, p_context, p_map, label_Bailout);}

TNode<Number> TorqueGeneratedExportedMacrosAssembler::ToInteger_Inline(TNode<Context> p_context, TNode<Object> p_input) {
return ToInteger_Inline_0(state_, p_context, p_input);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::RequireObjectCoercible(TNode<Context> p_context, TNode<Object> p_value, const char* p_name) {
return RequireObjectCoercible_0(state_, p_context, p_value, p_name);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::ChangeUintPtrNumberToUintPtr(TNode<Number> p_value) {
return ChangeUintPtrNumberToUintPtr_0(state_, p_value);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::ChangeSafeIntegerNumberToUintPtr(TNode<Number> p_value, compiler::CodeAssemblerLabel* label_IfUIntPtrOverflow) {
return ChangeSafeIntegerNumberToUintPtr_0(state_, p_value, label_IfUIntPtrOverflow);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::ToIndex(TNode<Context> p_context, TNode<Object> p_value, compiler::CodeAssemblerLabel* label_IfRangeError) {
return ToIndex_0(state_, p_context, p_value, label_IfRangeError);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::ConvertToRelativeIndex(TNode<Context> p_context, TNode<Object> p_index, TNode<UintPtrT> p_length) {
return ConvertToRelativeIndex_1(state_, p_context, p_index, p_length);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::ConvertToRelativeIndex(TNode<Number> p_indexNumber, TNode<UintPtrT> p_length) {
return ConvertToRelativeIndex_2(state_, p_indexNumber, p_length);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::ClampToIndexRange(TNode<Context> p_context, TNode<Object> p_index, TNode<UintPtrT> p_limit) {
return ClampToIndexRange_0(state_, p_context, p_index, p_limit);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::ClampToIndexRange(TNode<Number> p_indexNumber, TNode<UintPtrT> p_limit) {
return ClampToIndexRange_1(state_, p_indexNumber, p_limit);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsFastJSArray(TNode<Object> p_o, TNode<Context> p_context) {
return IsFastJSArray_0(state_, p_o, p_context);}

void TorqueGeneratedExportedMacrosAssembler::BranchIfFastJSArray(TNode<Object> p_o, TNode<Context> p_context, compiler::CodeAssemblerLabel* label_True, compiler::CodeAssemblerLabel* label_False) {
return BranchIfFastJSArray_0(state_, p_o, p_context, label_True, label_False);}

void TorqueGeneratedExportedMacrosAssembler::BranchIfFastJSArrayForRead(TNode<Object> p_o, TNode<Context> p_context, compiler::CodeAssemblerLabel* label_True, compiler::CodeAssemblerLabel* label_False) {
return BranchIfFastJSArrayForRead_0(state_, p_o, p_context, label_True, label_False);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsFastJSArrayWithNoCustomIteration(TNode<Context> p_context, TNode<Object> p_o) {
return IsFastJSArrayWithNoCustomIteration_0(state_, p_context, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsFastJSArrayForReadWithNoCustomIteration(TNode<Context> p_context, TNode<Object> p_o) {
return IsFastJSArrayForReadWithNoCustomIteration_0(state_, p_context, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsIntegerOrSomeInfinity(TNode<Object> p_o) {
return IsIntegerOrSomeInfinity_0(state_, p_o);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::ToStringImpl(TNode<Context> p_context, TNode<Object> p_o) {
return ToStringImpl_0(state_, p_context, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsAccessorInfo(TNode<HeapObject> p_o) {
return IsAccessorInfo_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsAccessorPair(TNode<HeapObject> p_o) {
return IsAccessorPair_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsAllocationSite(TNode<HeapObject> p_o) {
return IsAllocationSite_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsCell(TNode<HeapObject> p_o) {
return IsCell_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsCode(TNode<HeapObject> p_o) {
return IsCode_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsCodeDataContainer(TNode<HeapObject> p_o) {
return IsCodeDataContainer_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsContext(TNode<HeapObject> p_o) {
return IsContext_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsCoverageInfo(TNode<HeapObject> p_o) {
return IsCoverageInfo_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsDebugInfo(TNode<HeapObject> p_o) {
return IsDebugInfo_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsFixedDoubleArray(TNode<HeapObject> p_o) {
return IsFixedDoubleArray_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsFeedbackCell(TNode<HeapObject> p_o) {
return IsFeedbackCell_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsFeedbackVector(TNode<HeapObject> p_o) {
return IsFeedbackVector_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsHeapNumber(TNode<HeapObject> p_o) {
return IsHeapNumber_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsNativeContext(TNode<HeapObject> p_o) {
return IsNativeContext_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsNumber(TNode<Object> p_o) {
return IsNumber_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsPrivateSymbol(TNode<HeapObject> p_o) {
return IsPrivateSymbol_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsPromiseCapability(TNode<HeapObject> p_o) {
return IsPromiseCapability_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsPromiseFulfillReactionJobTask(TNode<HeapObject> p_o) {
return IsPromiseFulfillReactionJobTask_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsPromiseReaction(TNode<HeapObject> p_o) {
return IsPromiseReaction_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsPromiseRejectReactionJobTask(TNode<HeapObject> p_o) {
return IsPromiseRejectReactionJobTask_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsSharedFunctionInfo(TNode<HeapObject> p_o) {
return IsSharedFunctionInfo_0(state_, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsSymbol(TNode<HeapObject> p_o) {
return IsSymbol_0(state_, p_o);}

TorqueStructKeyValuePair TorqueGeneratedExportedMacrosAssembler::LoadKeyValuePairNoSideEffects(TNode<Context> p_context, TNode<Object> p_o, compiler::CodeAssemblerLabel* label_MayHaveSideEffects) {
return LoadKeyValuePairNoSideEffects_0(state_, p_context, p_o, label_MayHaveSideEffects);}

TorqueStructKeyValuePair TorqueGeneratedExportedMacrosAssembler::LoadKeyValuePair(TNode<Context> p_context, TNode<Object> p_o) {
return LoadKeyValuePair_0(state_, p_context, p_o);}

TNode<JSFunction> TorqueGeneratedExportedMacrosAssembler::LoadTargetFromFrame() {
return LoadTargetFromFrame_0(state_);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::DynamicCheckMaps(TNode<Map> p_actualMap, TNode<IntPtrT> p_slotIndex, TNode<Object> p_actualHandler) {
return DynamicCheckMaps_0(state_, p_actualMap, p_slotIndex, p_actualHandler);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::DynamicCheckMapsWithFeedbackVector(TNode<Map> p_actualMap, TNode<IntPtrT> p_slotIndex, TNode<Object> p_actualHandler, TNode<FeedbackVector> p_feedbackVector) {
return DynamicCheckMapsWithFeedbackVector_0(state_, p_actualMap, p_slotIndex, p_actualHandler, p_feedbackVector);}

void TorqueGeneratedExportedMacrosAssembler::CollectCallFeedback(TNode<Object> p_maybeTarget, std::function<TNode<Object>()> p_maybeReceiver, TNode<Context> p_context, TNode<HeapObject> p_maybeFeedbackVector, TNode<UintPtrT> p_slotId) {
return CollectCallFeedback_1(state_, p_maybeTarget, p_maybeReceiver, p_context, p_maybeFeedbackVector, p_slotId);}

void TorqueGeneratedExportedMacrosAssembler::CollectInstanceOfFeedback(TNode<Object> p_maybeTarget, TNode<Context> p_context, TNode<HeapObject> p_maybeFeedbackVector, TNode<UintPtrT> p_slotId) {
return CollectInstanceOfFeedback_1(state_, p_maybeTarget, p_context, p_maybeFeedbackVector, p_slotId);}

void TorqueGeneratedExportedMacrosAssembler::CollectConstructFeedback(TNode<Context> p_context, TNode<Object> p_target, TNode<Object> p_newTarget, TNode<HeapObject> p_maybeFeedbackVector, TNode<UintPtrT> p_slotId, UpdateFeedbackMode p_updateFeedbackMode, compiler::CodeAssemblerLabel* label_ConstructGeneric, compiler::CodeAssemblerLabel* label_ConstructArray, compiler::TypedCodeAssemblerVariable<AllocationSite>* label_ConstructArray_parameter_0) {
return CollectConstructFeedback_1(state_, p_context, p_target, p_newTarget, p_maybeFeedbackVector, p_slotId, p_updateFeedbackMode, label_ConstructGeneric, label_ConstructArray, label_ConstructArray_parameter_0);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::ForInNextSlow(TNode<Context> p_context, TNode<UintPtrT> p_slot, TNode<HeapObject> p_receiver, TNode<Object> p_key, TNode<Object> p_cacheType, TNode<HeapObject> p_maybeFeedbackVector, UpdateFeedbackMode p_guaranteedFeedback) {
return ForInNextSlow_0(state_, p_context, p_slot, p_receiver, p_key, p_cacheType, p_maybeFeedbackVector, p_guaranteedFeedback);}

void TorqueGeneratedExportedMacrosAssembler::IteratorCloseOnException(TNode<Context> p_context, TorqueStructIteratorRecord p_iterator) {
return IteratorCloseOnException_0(state_, p_context, p_iterator);}

TNode<Number> TorqueGeneratedExportedMacrosAssembler::MathPowImpl(TNode<Context> p_context, TNode<Object> p_base, TNode<Object> p_exponent) {
return MathPowImpl_0(state_, p_context, p_base, p_exponent);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::NumberToStringSmi(TNode<Int32T> p_x, TNode<Int32T> p_radix, compiler::CodeAssemblerLabel* label_Slow) {
return NumberToStringSmi_0(state_, p_x, p_radix, label_Slow);}

TNode<Context> TorqueGeneratedExportedMacrosAssembler::CreatePromiseCapabilitiesExecutorContext(TNode<NativeContext> p_nativeContext, TNode<PromiseCapability> p_capability) {
return CreatePromiseCapabilitiesExecutorContext_0(state_, p_nativeContext, p_capability);}

TNode<PromiseCapability> TorqueGeneratedExportedMacrosAssembler::CreatePromiseCapability(TNode<HeapObject> p_promise, TNode<HeapObject> p_resolve, TNode<HeapObject> p_reject) {
return CreatePromiseCapability_0(state_, p_promise, p_resolve, p_reject);}

TorqueStructPromiseResolvingFunctions TorqueGeneratedExportedMacrosAssembler::CreatePromiseResolvingFunctions(TNode<Context> p_context, TNode<JSPromise> p_promise, TNode<Oddball> p_debugEvent, TNode<NativeContext> p_nativeContext) {
return CreatePromiseResolvingFunctions_0(state_, p_context, p_promise, p_debugEvent, p_nativeContext);}

void TorqueGeneratedExportedMacrosAssembler::PerformPromiseThenImpl(TNode<Context> p_context, TNode<JSPromise> p_promise, TNode<HeapObject> p_onFulfilled, TNode<HeapObject> p_onRejected, TNode<HeapObject> p_resultPromiseOrCapability) {
return PerformPromiseThenImpl_0(state_, p_context, p_promise, p_onFulfilled, p_onRejected, p_resultPromiseOrCapability);}

TNode<Context> TorqueGeneratedExportedMacrosAssembler::CreatePromiseResolvingFunctionsContext(TNode<Context> p_context, TNode<JSPromise> p_promise, TNode<Oddball> p_debugEvent, TNode<NativeContext> p_nativeContext) {
return CreatePromiseResolvingFunctionsContext_0(state_, p_context, p_promise, p_debugEvent, p_nativeContext);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::PromiseHasHandler(TNode<JSPromise> p_promise) {
return PromiseHasHandler_0(state_, p_promise);}

void TorqueGeneratedExportedMacrosAssembler::PromiseInit(TNode<JSPromise> p_promise) {
return PromiseInit_0(state_, p_promise);}

void TorqueGeneratedExportedMacrosAssembler::RunContextPromiseHookInit(TNode<Context> p_context, TNode<JSPromise> p_promise, TNode<Object> p_parent) {
return RunContextPromiseHookInit_0(state_, p_context, p_promise, p_parent);}

void TorqueGeneratedExportedMacrosAssembler::RunContextPromiseHookResolve(TNode<Context> p_context, TNode<JSPromise> p_promise) {
return RunContextPromiseHookResolve_0(state_, p_context, p_promise);}

void TorqueGeneratedExportedMacrosAssembler::RunContextPromiseHookResolve(TNode<Context> p_context, TNode<JSPromise> p_promise, TNode<Uint32T> p_flags) {
return RunContextPromiseHookResolve_1(state_, p_context, p_promise, p_flags);}

void TorqueGeneratedExportedMacrosAssembler::RunContextPromiseHookBefore(TNode<Context> p_context, TNode<HeapObject> p_promiseOrCapability) {
return RunContextPromiseHookBefore_0(state_, p_context, p_promiseOrCapability);}

void TorqueGeneratedExportedMacrosAssembler::RunContextPromiseHookBefore(TNode<Context> p_context, TNode<HeapObject> p_promiseOrCapability, TNode<Uint32T> p_flags) {
return RunContextPromiseHookBefore_1(state_, p_context, p_promiseOrCapability, p_flags);}

void TorqueGeneratedExportedMacrosAssembler::RunContextPromiseHookAfter(TNode<Context> p_context, TNode<HeapObject> p_promiseOrCapability) {
return RunContextPromiseHookAfter_0(state_, p_context, p_promiseOrCapability);}

void TorqueGeneratedExportedMacrosAssembler::RunContextPromiseHookAfter(TNode<Context> p_context, TNode<HeapObject> p_promiseOrCapability, TNode<Uint32T> p_flags) {
return RunContextPromiseHookAfter_1(state_, p_context, p_promiseOrCapability, p_flags);}

TNode<JSPromise> TorqueGeneratedExportedMacrosAssembler::NewJSPromise(TNode<Context> p_context, TNode<Object> p_parent) {
return NewJSPromise_0(state_, p_context, p_parent);}

TNode<JSPromise> TorqueGeneratedExportedMacrosAssembler::NewJSPromise(TNode<Context> p_context) {
return NewJSPromise_1(state_, p_context);}

TNode<JSPromise> TorqueGeneratedExportedMacrosAssembler::NewJSPromise(TNode<Context> p_context, Promise::PromiseState p_status, TNode<Object> p_result) {
return NewJSPromise_2(state_, p_context, p_status, p_result);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::RegExpPrototypeExecBodyFast(TNode<Context> p_context, TNode<JSReceiver> p_receiver, TNode<String> p_string) {
return RegExpPrototypeExecBodyFast_0(state_, p_context, p_receiver, p_string);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::RegExpPrototypeMatchAllImpl(TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<Object> p_receiver, TNode<Object> p_string) {
return RegExpPrototypeMatchAllImpl_0(state_, p_context, p_nativeContext, p_receiver, p_string);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsFastRegExpPermissive(TNode<Context> p_context, TNode<HeapObject> p_o) {
return IsFastRegExpPermissive_0(state_, p_context, p_o);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::RegExpExec(TNode<Context> p_context, TNode<JSReceiver> p_receiver, TNode<String> p_string) {
return RegExpExec_0(state_, p_context, p_receiver, p_string);}

TNode<RegExpMatchInfo> TorqueGeneratedExportedMacrosAssembler::RegExpPrototypeExecBodyWithoutResultFast(TNode<Context> p_context, TNode<JSRegExp> p_regexp, TNode<String> p_string, compiler::CodeAssemblerLabel* label_IfDidNotMatch) {
return RegExpPrototypeExecBodyWithoutResultFast_0(state_, p_context, p_regexp, p_string, label_IfDidNotMatch);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadLastIndex(TNode<Context> p_context, TNode<Object> p_regexp, bool p_isFastPath) {
return LoadLastIndex_0(state_, p_context, p_regexp, p_isFastPath);}

TNode<Number> TorqueGeneratedExportedMacrosAssembler::LoadLastIndexAsLength(TNode<Context> p_context, TNode<JSRegExp> p_regexp, bool p_isFastPath) {
return LoadLastIndexAsLength_0(state_, p_context, p_regexp, p_isFastPath);}

void TorqueGeneratedExportedMacrosAssembler::StoreLastIndex(TNode<Context> p_context, TNode<Object> p_regexp, TNode<Number> p_value, bool p_isFastPath) {
return StoreLastIndex_0(state_, p_context, p_regexp, p_value, p_isFastPath);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsRegExp(TNode<Context> p_context, TNode<Object> p_obj) {
return IsRegExp_0(state_, p_context, p_obj);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::RegExpCreate(TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<Object> p_maybeString, TNode<String> p_flags) {
return RegExpCreate_0(state_, p_context, p_nativeContext, p_maybeString, p_flags);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::RegExpCreate(TNode<Context> p_context, TNode<Map> p_initialMap, TNode<Object> p_maybeString, TNode<String> p_flags) {
return RegExpCreate_1(state_, p_context, p_initialMap, p_maybeString, p_flags);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsWhiteSpaceOrLineTerminator(TNode<Uint16T> p_charCode) {
return IsWhiteSpaceOrLineTerminator_0(state_, p_charCode);}

TNode<JSTypedArray> TorqueGeneratedExportedMacrosAssembler::TypedArrayCreateByLength(TNode<Context> p_context, TNode<JSReceiver> p_constructor, TNode<Number> p_length, const char* p_methodName) {
return TypedArrayCreateByLength_0(state_, p_context, p_constructor, p_length, p_methodName);}

TNode<JSTypedArray> TorqueGeneratedExportedMacrosAssembler::TypedArraySpeciesCreateByLength(TNode<Context> p_context, const char* p_methodName, TNode<JSTypedArray> p_exemplar, TNode<UintPtrT> p_length) {
return TypedArraySpeciesCreateByLength_0(state_, p_context, p_methodName, p_exemplar, p_length);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsJSArgumentsObjectWithLength(TNode<Context> p_context, TNode<Object> p_o) {
return IsJSArgumentsObjectWithLength_0(state_, p_context, p_o);}

TNode<JSArray> TorqueGeneratedExportedMacrosAssembler::EmitFastNewAllArguments(TNode<Context> p_context, TNode<RawPtrT> p_frame, TNode<IntPtrT> p_argc) {
return EmitFastNewAllArguments_0(state_, p_context, p_frame, p_argc);}

TNode<JSArray> TorqueGeneratedExportedMacrosAssembler::EmitFastNewRestArguments(TNode<Context> p_context, TNode<JSFunction> p__f) {
return EmitFastNewRestArguments_0(state_, p_context, p__f);}

TNode<JSArgumentsObject> TorqueGeneratedExportedMacrosAssembler::EmitFastNewStrictArguments(TNode<Context> p_context, TNode<JSFunction> p__f) {
return EmitFastNewStrictArguments_0(state_, p_context, p__f);}

TNode<JSArgumentsObject> TorqueGeneratedExportedMacrosAssembler::EmitFastNewSloppyArguments(TNode<Context> p_context, TNode<JSFunction> p_f) {
return EmitFastNewSloppyArguments_0(state_, p_context, p_f);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::SloppyArgumentsLoad(TNode<JSObject> p_receiver, TNode<Object> p_keyObject, compiler::CodeAssemblerLabel* label_Bailout) {
return SloppyArgumentsLoad_0(state_, p_receiver, p_keyObject, label_Bailout);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::SloppyArgumentsHas(TNode<JSObject> p_receiver, TNode<Object> p_keyObject, compiler::CodeAssemblerLabel* label_Bailout) {
return SloppyArgumentsHas_0(state_, p_receiver, p_keyObject, label_Bailout);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::SloppyArgumentsStore(TNode<JSObject> p_receiver, TNode<Object> p_keyObject, TNode<Object> p_value, compiler::CodeAssemblerLabel* label_Bailout) {
return SloppyArgumentsStore_0(state_, p_receiver, p_keyObject, p_value, label_Bailout);}

TNode<Context> TorqueGeneratedExportedMacrosAssembler::AllocateSyntheticFunctionContext(TNode<NativeContext> p_nativeContext, int31_t p_slots) {
return AllocateSyntheticFunctionContext_0(state_, p_nativeContext, p_slots);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadContextElement(TNode<Context> p_c, TNode<IntPtrT> p_i) {
return LoadContextElement_0(state_, p_c, p_i);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadContextElement(TNode<Context> p_c, TNode<Smi> p_i) {
return LoadContextElement_1(state_, p_c, p_i);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadContextElement(TNode<Context> p_c, int32_t p_i) {
return LoadContextElement_2(state_, p_c, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreContextElement(TNode<Context> p_c, TNode<IntPtrT> p_i, TNode<Object> p_o) {
return StoreContextElement_0(state_, p_c, p_i, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreContextElement(TNode<Context> p_c, TNode<Smi> p_i, TNode<Object> p_o) {
return StoreContextElement_1(state_, p_c, p_i, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreContextElement(TNode<Context> p_c, int32_t p_i, TNode<Object> p_o) {
return StoreContextElement_2(state_, p_c, p_i, p_o);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsDetachedBuffer(TNode<JSArrayBuffer> p_buffer) {
return IsDetachedBuffer_0(state_, p_buffer);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsSharedArrayBuffer(TNode<JSArrayBuffer> p_buffer) {
return IsSharedArrayBuffer_0(state_, p_buffer);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsResizableArrayBuffer(TNode<JSArrayBuffer> p_buffer) {
return IsResizableArrayBuffer_0(state_, p_buffer);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsVariableLengthTypedArray(TNode<JSTypedArray> p_array) {
return IsVariableLengthTypedArray_0(state_, p_array);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsLengthTrackingTypedArray(TNode<JSTypedArray> p_array) {
return IsLengthTrackingTypedArray_0(state_, p_array);}

TNode<JSArrayIterator> TorqueGeneratedExportedMacrosAssembler::CreateArrayIterator(TNode<NativeContext> p_context, TNode<JSReceiver> p_array, IterationKind p_kind) {
return CreateArrayIterator_0(state_, p_context, p_array, p_kind);}

TNode<PrototypeInfo> TorqueGeneratedExportedMacrosAssembler::LoadMapPrototypeInfo(TNode<Map> p_m, compiler::CodeAssemblerLabel* label_HasNoPrototypeInfo) {
return LoadMapPrototypeInfo_0(state_, p_m, label_HasNoPrototypeInfo);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::IsSimpleObjectMap(TNode<Map> p_map) {
return IsSimpleObjectMap_0(state_, p_map);}

TNode<SmallOrderedHashSet> TorqueGeneratedExportedMacrosAssembler::AllocateSmallOrderedHashSet(TNode<IntPtrT> p_capacity) {
return AllocateSmallOrderedHashSet_0(state_, p_capacity);}

TNode<SmallOrderedHashMap> TorqueGeneratedExportedMacrosAssembler::AllocateSmallOrderedHashMap(TNode<IntPtrT> p_capacity) {
return AllocateSmallOrderedHashMap_0(state_, p_capacity);}

TNode<IntPtrT> TorqueGeneratedExportedMacrosAssembler::IndexOfLocalName(TNode<ScopeInfo> p_scopeInfo, TNode<Name> p_name, compiler::CodeAssemblerLabel* label_NotFound) {
return IndexOfLocalName_0(state_, p_scopeInfo, p_name, label_NotFound);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::AllocateSeqOneByteString(TNode<Uint32T> p_length) {
return AllocateSeqOneByteString_0(state_, p_length);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::AllocateSeqTwoByteString(TNode<Uint32T> p_length) {
return AllocateSeqTwoByteString_0(state_, p_length);}

TNode<IntPtrT> TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryCapacityFor(TNode<IntPtrT> p_atLeastSpaceFor) {
return SwissNameDictionaryCapacityFor_0(state_, p_atLeastSpaceFor);}

TNode<IntPtrT> TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryMaxUsableCapacity(TNode<IntPtrT> p_capacity) {
return SwissNameDictionaryMaxUsableCapacity_0(state_, p_capacity);}

TNode<IntPtrT> TorqueGeneratedExportedMacrosAssembler::SwissNameDictionarySizeFor(TNode<IntPtrT> p_capacity) {
return SwissNameDictionarySizeFor_0(state_, p_capacity);}

TNode<IntPtrT> TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryMetaTableSizePerEntryFor(TNode<IntPtrT> p_capacity) {
return SwissNameDictionaryMetaTableSizePerEntryFor_0(state_, p_capacity);}

TNode<IntPtrT> TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryMetaTableSizeFor(TNode<IntPtrT> p_capacity) {
return SwissNameDictionaryMetaTableSizeFor_0(state_, p_capacity);}

TNode<IntPtrT> TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryDataTableStartOffsetMT() {
return SwissNameDictionaryDataTableStartOffsetMT_0(state_);}

TNode<IntPtrT> TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryCtrlTableStartOffsetMT(TNode<IntPtrT> p_capacity) {
return SwissNameDictionaryCtrlTableStartOffsetMT_0(state_, p_capacity);}

void TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryDelete(TNode<SwissNameDictionary> p_table, TNode<IntPtrT> p_entry, compiler::CodeAssemblerLabel* label_Shrunk, compiler::TypedCodeAssemblerVariable<SwissNameDictionary>* label_Shrunk_parameter_0) {
return SwissNameDictionaryDelete_0(state_, p_table, p_entry, label_Shrunk, label_Shrunk_parameter_0);}

void TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryFindEntrySIMD(TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound) {
return SwissNameDictionaryFindEntrySIMD_0(state_, p_table, p_key, label_Found, label_Found_parameter_0, label_NotFound);}

void TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryFindEntryPortable(TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound) {
return SwissNameDictionaryFindEntryPortable_0(state_, p_table, p_key, label_Found, label_Found_parameter_0, label_NotFound);}

void TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryAddSIMD(TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout) {
return SwissNameDictionaryAddSIMD_0(state_, p_table, p_key, p_value, p_propertyDetails, label_Bailout);}

void TorqueGeneratedExportedMacrosAssembler::SwissNameDictionaryAddPortable(TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout) {
return SwissNameDictionaryAddPortable_0(state_, p_table, p_key, p_value, p_propertyDetails, label_Bailout);}

void TorqueGeneratedExportedMacrosAssembler::TestConstexpr1() {
return TestConstexpr1_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestConstexprIf() {
return TestConstexprIf_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestConstexprReturn() {
return TestConstexprReturn_0(state_);}

TNode<Oddball> TorqueGeneratedExportedMacrosAssembler::TestGotoLabel() {
return TestGotoLabel_0(state_);}

TNode<Oddball> TorqueGeneratedExportedMacrosAssembler::TestGotoLabelWithOneParameter() {
return TestGotoLabelWithOneParameter_0(state_);}

TNode<Oddball> TorqueGeneratedExportedMacrosAssembler::TestGotoLabelWithTwoParameters() {
return TestGotoLabelWithTwoParameters_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestBuiltinSpecialization() {
return TestBuiltinSpecialization_0(state_);}

TNode<Oddball> TorqueGeneratedExportedMacrosAssembler::TestPartiallyUnusedLabel() {
return TestPartiallyUnusedLabel_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestMacroSpecialization() {
return TestMacroSpecialization_0(state_);}

TNode<Oddball> TorqueGeneratedExportedMacrosAssembler::TestFunctionPointers(TNode<Context> p_context) {
return TestFunctionPointers_0(state_, p_context);}

TNode<Oddball> TorqueGeneratedExportedMacrosAssembler::TestVariableRedeclaration(TNode<Context> p_context) {
return TestVariableRedeclaration_0(state_, p_context);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::TestTernaryOperator(TNode<Smi> p_x) {
return TestTernaryOperator_0(state_, p_x);}

void TorqueGeneratedExportedMacrosAssembler::TestFunctionPointerToGeneric() {
return TestFunctionPointerToGeneric_0(state_);}

TNode<BuiltinPtr> TorqueGeneratedExportedMacrosAssembler::TestTypeAlias(TNode<BuiltinPtr> p_x) {
return TestTypeAlias_0(state_, p_x);}

TNode<Oddball> TorqueGeneratedExportedMacrosAssembler::TestUnsafeCast(TNode<Context> p_context, TNode<Number> p_n) {
return TestUnsafeCast_0(state_, p_context, p_n);}

void TorqueGeneratedExportedMacrosAssembler::TestHexLiteral() {
return TestHexLiteral_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestLargeIntegerLiterals(TNode<Context> p_c) {
return TestLargeIntegerLiterals_0(state_, p_c);}

void TorqueGeneratedExportedMacrosAssembler::TestMultilineAssert() {
return TestMultilineAssert_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestNewlineInString() {
return TestNewlineInString_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestModuleConstBindings() {
return TestModuleConstBindings_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestLocalConstBindings() {
return TestLocalConstBindings_0(state_);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::TestStruct1(TorqueStructTestStructA_0 p_i) {
return TestStruct1_0(state_, p_i);}

TorqueStructTestStructA_0 TorqueGeneratedExportedMacrosAssembler::TestStruct2(TNode<Context> p_context) {
return TestStruct2_0(state_, p_context);}

TorqueStructTestStructA_0 TorqueGeneratedExportedMacrosAssembler::TestStruct3(TNode<Context> p_context) {
return TestStruct3_0(state_, p_context);}

TorqueStructTestStructC_0 TorqueGeneratedExportedMacrosAssembler::TestStruct4(TNode<Context> p_context) {
return TestStruct4_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::CallTestStructInLabel(TNode<Context> p_context) {
return CallTestStructInLabel_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestForLoop() {
return TestForLoop_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestSubtyping(TNode<Smi> p_x) {
return TestSubtyping_0(state_, p_x);}

void TorqueGeneratedExportedMacrosAssembler::TestTypeswitch(TNode<Context> p_context) {
return TestTypeswitch_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestTypeswitchAsanLsanFailure(TNode<Context> p_context, TNode<Object> p_obj) {
return TestTypeswitchAsanLsanFailure_0(state_, p_context, p_obj);}

void TorqueGeneratedExportedMacrosAssembler::TestGenericOverload(TNode<Context> p_context) {
return TestGenericOverload_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestEquality(TNode<Context> p_context) {
return TestEquality_0(state_, p_context);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::TestOrAnd(TNode<BoolT> p_x, TNode<BoolT> p_y, TNode<BoolT> p_z) {
return TestOrAnd_0(state_, p_x, p_y, p_z);}

TNode<BoolT> TorqueGeneratedExportedMacrosAssembler::TestAndOr(TNode<BoolT> p_x, TNode<BoolT> p_y, TNode<BoolT> p_z) {
return TestAndOr_0(state_, p_x, p_y, p_z);}

void TorqueGeneratedExportedMacrosAssembler::TestLogicalOperators() {
return TestLogicalOperators_0(state_);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::TestCall(TNode<Smi> p_i, compiler::CodeAssemblerLabel* label_A) {
return TestCall_0(state_, p_i, label_A);}

void TorqueGeneratedExportedMacrosAssembler::TestOtherwiseWithCode1() {
return TestOtherwiseWithCode1_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestOtherwiseWithCode2() {
return TestOtherwiseWithCode2_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestOtherwiseWithCode3() {
return TestOtherwiseWithCode3_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestForwardLabel() {
return TestForwardLabel_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestQualifiedAccess(TNode<Context> p_context) {
return TestQualifiedAccess_0(state_, p_context);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::TestCatch1(TNode<Context> p_context) {
return TestCatch1_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestCatch2Wrapper(TNode<Context> p_context) {
return TestCatch2Wrapper_0(state_, p_context);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::TestCatch2(TNode<Context> p_context) {
return TestCatch2_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestCatch3WrapperWithLabel(TNode<Context> p_context, compiler::CodeAssemblerLabel* label__Abort) {
return TestCatch3WrapperWithLabel_0(state_, p_context, label__Abort);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::TestCatch3(TNode<Context> p_context) {
return TestCatch3_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestIterator(TNode<Context> p_context, TNode<JSReceiver> p_o, TNode<Map> p_map) {
return TestIterator_0(state_, p_context, p_o, p_map);}

void TorqueGeneratedExportedMacrosAssembler::TestFrame1(TNode<Context> p_context) {
return TestFrame1_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestNew(TNode<Context> p_context) {
return TestNew_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestStructConstructor(TNode<Context> p_context) {
return TestStructConstructor_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestInternalClass(TNode<Context> p_context) {
return TestInternalClass_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestConstInStructs() {
return TestConstInStructs_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestParentFrameArguments(TNode<Context> p_context) {
return TestParentFrameArguments_0(state_, p_context);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::TestNewFixedArrayFromSpread(TNode<Context> p_context) {
return TestNewFixedArrayFromSpread_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestReferences() {
return TestReferences_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestSlices() {
return TestSlices_0(state_);}

TNode<Oddball> TorqueGeneratedExportedMacrosAssembler::TestSliceEnumeration(TNode<Context> p_context) {
return TestSliceEnumeration_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestStaticAssert() {
return TestStaticAssert_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestLoadEliminationFixed(TNode<Context> p_context) {
return TestLoadEliminationFixed_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestLoadEliminationVariable(TNode<Context> p_context) {
return TestLoadEliminationVariable_0(state_, p_context);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::TestRedundantArrayElementCheck(TNode<Context> p_context) {
return TestRedundantArrayElementCheck_0(state_, p_context);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::TestRedundantSmiCheck(TNode<Context> p_context) {
return TestRedundantSmiCheck_0(state_, p_context);}

TNode<IntPtrT> TorqueGeneratedExportedMacrosAssembler::TestGenericStruct1() {
return TestGenericStruct1_0(state_);}

TorqueStructTestTuple_TestTuple_intptr_Smi_TestTuple_Smi_intptr_0 TorqueGeneratedExportedMacrosAssembler::TestGenericStruct2() {
return TestGenericStruct2_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestBranchOnBoolOptimization(TNode<Context> p_context, TNode<Smi> p_input) {
return TestBranchOnBoolOptimization_0(state_, p_context, p_input);}

void TorqueGeneratedExportedMacrosAssembler::TestBitFieldLoad(TNode<Uint8T> p_val, TNode<BoolT> p_expectedA, TNode<Uint16T> p_expectedB, TNode<Uint32T> p_expectedC, TNode<BoolT> p_expectedD) {
return TestBitFieldLoad_0(state_, p_val, p_expectedA, p_expectedB, p_expectedC, p_expectedD);}

void TorqueGeneratedExportedMacrosAssembler::TestBitFieldStore(TNode<Uint8T> p_val) {
return TestBitFieldStore_0(state_, p_val);}

void TorqueGeneratedExportedMacrosAssembler::TestBitFieldInit(TNode<BoolT> p_a, TNode<Uint16T> p_b, TNode<Uint32T> p_c, TNode<BoolT> p_d) {
return TestBitFieldInit_0(state_, p_a, p_b, p_c, p_d);}

void TorqueGeneratedExportedMacrosAssembler::TestBitFieldUintptrOps(TNode<Uint32T> p_val2, TNode<UintPtrT> p_val3) {
return TestBitFieldUintptrOps_0(state_, p_val2, p_val3);}

void TorqueGeneratedExportedMacrosAssembler::TestBitFieldMultipleFlags(TNode<BoolT> p_a, TNode<Int32T> p_b, TNode<BoolT> p_c) {
return TestBitFieldMultipleFlags_0(state_, p_a, p_b, p_c);}

void TorqueGeneratedExportedMacrosAssembler::TestFullyGeneratedClassWithElements() {
return TestFullyGeneratedClassWithElements_0(state_);}

TNode<ExportedSubClass> TorqueGeneratedExportedMacrosAssembler::TestFullyGeneratedClassFromCpp() {
return TestFullyGeneratedClassFromCpp_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestGeneratedCastOperators(TNode<Context> p_context) {
return TestGeneratedCastOperators_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestNewPretenured(TNode<Context> p_context) {
return TestNewPretenured_0(state_, p_context);}

void TorqueGeneratedExportedMacrosAssembler::TestWord8Phi() {
return TestWord8Phi_0(state_);}

void TorqueGeneratedExportedMacrosAssembler::TestOffHeapSlice(TNode<RawPtrT> p_ptr, TNode<IntPtrT> p_length) {
return TestOffHeapSlice_0(state_, p_ptr, p_length);}

void TorqueGeneratedExportedMacrosAssembler::TestCallMultiReturnBuiltin(TNode<Context> p_context) {
return TestCallMultiReturnBuiltin_0(state_, p_context);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::TestRunLazyTwice(std::function<TNode<Smi>()> p_lazySmi) {
return TestRunLazyTwice_0(state_, p_lazySmi);}

void TorqueGeneratedExportedMacrosAssembler::TestCreateLazyNodeFromTorque() {
return TestCreateLazyNodeFromTorque_0(state_);}

TNode<Map> TorqueGeneratedExportedMacrosAssembler::LoadHeapObjectMap(TNode<HeapObject> p_o) {
return LoadHeapObjectMap_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadContextLength(TNode<Context> p_o) {
return LoadContextLength_0(state_, p_o);}

TorqueStructSlice_Object_MutableReference_Object_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceContextElements(TNode<Context> p_o) {
return FieldSliceContextElements_0(state_, p_o);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadContextElements(TNode<Context> p_o, TNode<IntPtrT> p_i) {
return LoadContextElements_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreContextElements(TNode<Context> p_o, TNode<IntPtrT> p_i, TNode<Object> p_v) {
return StoreContextElements_0(state_, p_o, p_i, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSReceiverPropertiesOrHash(TNode<JSReceiver> p_o) {
return LoadJSReceiverPropertiesOrHash_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSReceiverPropertiesOrHash(TNode<JSReceiver> p_o, TNode<Object> p_v) {
return StoreJSReceiverPropertiesOrHash_0(state_, p_o, p_v);}

TNode<Float64T> TorqueGeneratedExportedMacrosAssembler::LoadHeapNumberValue(TNode<HeapNumber> p_o) {
return LoadHeapNumberValue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreHeapNumberValue(TNode<HeapNumber> p_o, TNode<Float64T> p_v) {
return StoreHeapNumberValue_0(state_, p_o, p_v);}

TNode<Uint32T> TorqueGeneratedExportedMacrosAssembler::LoadNameRawHashField(TNode<Name> p_o) {
return LoadNameRawHashField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreNameRawHashField(TNode<Name> p_o, TNode<Uint32T> p_v) {
return StoreNameRawHashField_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadStringLength(TNode<String> p_o) {
return LoadStringLength_0(state_, p_o);}

TNode<Uint32T> TorqueGeneratedExportedMacrosAssembler::LoadSymbolFlags(TNode<Symbol> p_o) {
return LoadSymbolFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSymbolFlags(TNode<Symbol> p_o, TNode<Uint32T> p_v) {
return StoreSymbolFlags_0(state_, p_o, p_v);}

TNode<PrimitiveHeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSymbolDescription(TNode<Symbol> p_o) {
return LoadSymbolDescription_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSymbolDescription(TNode<Symbol> p_o, TNode<PrimitiveHeapObject> p_v) {
return StoreSymbolDescription_0(state_, p_o, p_v);}

TNode<Float64T> TorqueGeneratedExportedMacrosAssembler::LoadOddballToNumberRaw(TNode<Oddball> p_o) {
return LoadOddballToNumberRaw_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOddballToNumberRaw(TNode<Oddball> p_o, TNode<Float64T> p_v) {
return StoreOddballToNumberRaw_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadOddballToString(TNode<Oddball> p_o) {
return LoadOddballToString_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOddballToString(TNode<Oddball> p_o, TNode<String> p_v) {
return StoreOddballToString_0(state_, p_o, p_v);}

TNode<Number> TorqueGeneratedExportedMacrosAssembler::LoadOddballToNumber(TNode<Oddball> p_o) {
return LoadOddballToNumber_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOddballToNumber(TNode<Oddball> p_o, TNode<Number> p_v) {
return StoreOddballToNumber_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadOddballTypeOf(TNode<Oddball> p_o) {
return LoadOddballTypeOf_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOddballTypeOf(TNode<Oddball> p_o, TNode<String> p_v) {
return StoreOddballTypeOf_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadOddballKind(TNode<Oddball> p_o) {
return LoadOddballKind_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOddballKind(TNode<Oddball> p_o, TNode<Smi> p_v) {
return StoreOddballKind_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadFixedArrayBaseLength(TNode<FixedArrayBase> p_o) {
return LoadFixedArrayBaseLength_0(state_, p_o);}

TorqueStructSlice_Object_MutableReference_Object_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceFixedArrayObjects(TNode<FixedArray> p_o) {
return FieldSliceFixedArrayObjects_0(state_, p_o);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadFixedArrayObjects(TNode<FixedArray> p_o, TNode<IntPtrT> p_i) {
return LoadFixedArrayObjects_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreFixedArrayObjects(TNode<FixedArray> p_o, TNode<IntPtrT> p_i, TNode<Object> p_v) {
return StoreFixedArrayObjects_0(state_, p_o, p_i, p_v);}

TNode<FixedArrayBase> TorqueGeneratedExportedMacrosAssembler::LoadJSObjectElements(TNode<JSObject> p_o) {
return LoadJSObjectElements_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSObjectElements(TNode<JSObject> p_o, TNode<FixedArrayBase> p_v) {
return StoreJSObjectElements_0(state_, p_o, p_v);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceByteArrayBytes(TNode<ByteArray> p_o) {
return FieldSliceByteArrayBytes_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadByteArrayBytes(TNode<ByteArray> p_o, TNode<IntPtrT> p_i) {
return LoadByteArrayBytes_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreByteArrayBytes(TNode<ByteArray> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreByteArrayBytes_0(state_, p_o, p_i, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadWeakFixedArrayLength(TNode<WeakFixedArray> p_o) {
return LoadWeakFixedArrayLength_0(state_, p_o);}

TorqueStructSlice_MaybeObject_MutableReference_MaybeObject_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceWeakFixedArrayObjects(TNode<WeakFixedArray> p_o) {
return FieldSliceWeakFixedArrayObjects_0(state_, p_o);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadWeakFixedArrayObjects(TNode<WeakFixedArray> p_o, TNode<IntPtrT> p_i) {
return LoadWeakFixedArrayObjects_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakFixedArrayObjects(TNode<WeakFixedArray> p_o, TNode<IntPtrT> p_i, TNode<MaybeObject> p_v) {
return StoreWeakFixedArrayObjects_0(state_, p_o, p_i, p_v);}

TNode<ExternalPointerT> TorqueGeneratedExportedMacrosAssembler::LoadForeignForeignAddress(TNode<Foreign> p_o) {
return LoadForeignForeignAddress_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreForeignForeignAddress(TNode<Foreign> p_o, TNode<ExternalPointerT> p_v) {
return StoreForeignForeignAddress_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSProxyTarget(TNode<JSProxy> p_o) {
return LoadJSProxyTarget_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSProxyTarget(TNode<JSProxy> p_o, TNode<HeapObject> p_v) {
return StoreJSProxyTarget_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSProxyHandler(TNode<JSProxy> p_o) {
return LoadJSProxyHandler_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSProxyHandler(TNode<JSProxy> p_o, TNode<HeapObject> p_v) {
return StoreJSProxyHandler_0(state_, p_o, p_v);}

TNode<SharedFunctionInfo> TorqueGeneratedExportedMacrosAssembler::LoadJSFunctionSharedFunctionInfo(TNode<JSFunction> p_o) {
return LoadJSFunctionSharedFunctionInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFunctionSharedFunctionInfo(TNode<JSFunction> p_o, TNode<SharedFunctionInfo> p_v) {
return StoreJSFunctionSharedFunctionInfo_0(state_, p_o, p_v);}

TNode<Context> TorqueGeneratedExportedMacrosAssembler::LoadJSFunctionContext(TNode<JSFunction> p_o) {
return LoadJSFunctionContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFunctionContext(TNode<JSFunction> p_o, TNode<Context> p_v) {
return StoreJSFunctionContext_0(state_, p_o, p_v);}

TNode<FeedbackCell> TorqueGeneratedExportedMacrosAssembler::LoadJSFunctionFeedbackCell(TNode<JSFunction> p_o) {
return LoadJSFunctionFeedbackCell_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFunctionFeedbackCell(TNode<JSFunction> p_o, TNode<FeedbackCell> p_v) {
return StoreJSFunctionFeedbackCell_0(state_, p_o, p_v);}

TNode<Code> TorqueGeneratedExportedMacrosAssembler::LoadJSFunctionCode(TNode<JSFunction> p_o) {
return LoadJSFunctionCode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFunctionCode(TNode<JSFunction> p_o, TNode<Code> p_v) {
return StoreJSFunctionCode_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSFunctionPrototypeOrInitialMap(TNode<JSFunction> p_o) {
return LoadJSFunctionPrototypeOrInitialMap_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFunctionPrototypeOrInitialMap(TNode<JSFunction> p_o, TNode<HeapObject> p_v) {
return StoreJSFunctionPrototypeOrInitialMap_0(state_, p_o, p_v);}

TNode<JSReceiver> TorqueGeneratedExportedMacrosAssembler::LoadJSBoundFunctionBoundTargetFunction(TNode<JSBoundFunction> p_o) {
return LoadJSBoundFunctionBoundTargetFunction_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSBoundFunctionBoundTargetFunction(TNode<JSBoundFunction> p_o, TNode<JSReceiver> p_v) {
return StoreJSBoundFunctionBoundTargetFunction_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSBoundFunctionBoundThis(TNode<JSBoundFunction> p_o) {
return LoadJSBoundFunctionBoundThis_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSBoundFunctionBoundThis(TNode<JSBoundFunction> p_o, TNode<Object> p_v) {
return StoreJSBoundFunctionBoundThis_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadJSBoundFunctionBoundArguments(TNode<JSBoundFunction> p_o) {
return LoadJSBoundFunctionBoundArguments_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSBoundFunctionBoundArguments(TNode<JSBoundFunction> p_o, TNode<FixedArray> p_v) {
return StoreJSBoundFunctionBoundArguments_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadMapInstanceSizeInWords(TNode<Map> p_o) {
return LoadMapInstanceSizeInWords_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapInstanceSizeInWords(TNode<Map> p_o, TNode<Uint8T> p_v) {
return StoreMapInstanceSizeInWords_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadMapInObjectPropertiesStartOrConstructorFunctionIndex(TNode<Map> p_o) {
return LoadMapInObjectPropertiesStartOrConstructorFunctionIndex_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapInObjectPropertiesStartOrConstructorFunctionIndex(TNode<Map> p_o, TNode<Uint8T> p_v) {
return StoreMapInObjectPropertiesStartOrConstructorFunctionIndex_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadMapUsedOrUnusedInstanceSizeInWords(TNode<Map> p_o) {
return LoadMapUsedOrUnusedInstanceSizeInWords_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapUsedOrUnusedInstanceSizeInWords(TNode<Map> p_o, TNode<Uint8T> p_v) {
return StoreMapUsedOrUnusedInstanceSizeInWords_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadMapVisitorId(TNode<Map> p_o) {
return LoadMapVisitorId_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapVisitorId(TNode<Map> p_o, TNode<Uint8T> p_v) {
return StoreMapVisitorId_0(state_, p_o, p_v);}

TNode<Uint16T> TorqueGeneratedExportedMacrosAssembler::LoadMapInstanceType(TNode<Map> p_o) {
return LoadMapInstanceType_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapInstanceType(TNode<Map> p_o, TNode<Uint16T> p_v) {
return StoreMapInstanceType_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadMapBitField(TNode<Map> p_o) {
return LoadMapBitField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapBitField(TNode<Map> p_o, TNode<Uint8T> p_v) {
return StoreMapBitField_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadMapBitField2(TNode<Map> p_o) {
return LoadMapBitField2_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapBitField2(TNode<Map> p_o, TNode<Uint8T> p_v) {
return StoreMapBitField2_0(state_, p_o, p_v);}

TNode<Uint32T> TorqueGeneratedExportedMacrosAssembler::LoadMapBitField3(TNode<Map> p_o) {
return LoadMapBitField3_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapBitField3(TNode<Map> p_o, TNode<Uint32T> p_v) {
return StoreMapBitField3_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadMapPrototype(TNode<Map> p_o) {
return LoadMapPrototype_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapPrototype(TNode<Map> p_o, TNode<HeapObject> p_v) {
return StoreMapPrototype_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadMapConstructorOrBackPointerOrNativeContext(TNode<Map> p_o) {
return LoadMapConstructorOrBackPointerOrNativeContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapConstructorOrBackPointerOrNativeContext(TNode<Map> p_o, TNode<Object> p_v) {
return StoreMapConstructorOrBackPointerOrNativeContext_0(state_, p_o, p_v);}

TNode<DescriptorArray> TorqueGeneratedExportedMacrosAssembler::LoadMapInstanceDescriptors(TNode<Map> p_o) {
return LoadMapInstanceDescriptors_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapInstanceDescriptors(TNode<Map> p_o, TNode<DescriptorArray> p_v) {
return StoreMapInstanceDescriptors_0(state_, p_o, p_v);}

TNode<WeakFixedArray> TorqueGeneratedExportedMacrosAssembler::LoadMapDependentCode(TNode<Map> p_o) {
return LoadMapDependentCode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapDependentCode(TNode<Map> p_o, TNode<WeakFixedArray> p_v) {
return StoreMapDependentCode_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadMapPrototypeValidityCell(TNode<Map> p_o) {
return LoadMapPrototypeValidityCell_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapPrototypeValidityCell(TNode<Map> p_o, TNode<Object> p_v) {
return StoreMapPrototypeValidityCell_0(state_, p_o, p_v);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadMapTransitionsOrPrototypeInfo(TNode<Map> p_o) {
return LoadMapTransitionsOrPrototypeInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMapTransitionsOrPrototypeInfo(TNode<Map> p_o, TNode<MaybeObject> p_v) {
return StoreMapTransitionsOrPrototypeInfo_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadWeakCellFinalizationRegistry(TNode<WeakCell> p_o) {
return LoadWeakCellFinalizationRegistry_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakCellFinalizationRegistry(TNode<WeakCell> p_o, TNode<HeapObject> p_v) {
return StoreWeakCellFinalizationRegistry_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadWeakCellTarget(TNode<WeakCell> p_o) {
return LoadWeakCellTarget_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakCellTarget(TNode<WeakCell> p_o, TNode<HeapObject> p_v) {
return StoreWeakCellTarget_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadWeakCellUnregisterToken(TNode<WeakCell> p_o) {
return LoadWeakCellUnregisterToken_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakCellUnregisterToken(TNode<WeakCell> p_o, TNode<HeapObject> p_v) {
return StoreWeakCellUnregisterToken_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadWeakCellHoldings(TNode<WeakCell> p_o) {
return LoadWeakCellHoldings_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakCellHoldings(TNode<WeakCell> p_o, TNode<Object> p_v) {
return StoreWeakCellHoldings_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadWeakCellPrev(TNode<WeakCell> p_o) {
return LoadWeakCellPrev_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakCellPrev(TNode<WeakCell> p_o, TNode<HeapObject> p_v) {
return StoreWeakCellPrev_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadWeakCellNext(TNode<WeakCell> p_o) {
return LoadWeakCellNext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakCellNext(TNode<WeakCell> p_o, TNode<HeapObject> p_v) {
return StoreWeakCellNext_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadWeakCellKeyListPrev(TNode<WeakCell> p_o) {
return LoadWeakCellKeyListPrev_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakCellKeyListPrev(TNode<WeakCell> p_o, TNode<HeapObject> p_v) {
return StoreWeakCellKeyListPrev_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadWeakCellKeyListNext(TNode<WeakCell> p_o) {
return LoadWeakCellKeyListNext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakCellKeyListNext(TNode<WeakCell> p_o, TNode<HeapObject> p_v) {
return StoreWeakCellKeyListNext_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSPromiseReactionsOrResult(TNode<JSPromise> p_o) {
return LoadJSPromiseReactionsOrResult_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSPromiseReactionsOrResult(TNode<JSPromise> p_o, TNode<Object> p_v) {
return StoreJSPromiseReactionsOrResult_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSPromiseFlags(TNode<JSPromise> p_o) {
return LoadJSPromiseFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSPromiseFlags(TNode<JSPromise> p_o, TNode<Smi> p_v) {
return StoreJSPromiseFlags_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadPromiseCapabilityPromise(TNode<PromiseCapability> p_o) {
return LoadPromiseCapabilityPromise_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseCapabilityPromise(TNode<PromiseCapability> p_o, TNode<HeapObject> p_v) {
return StorePromiseCapabilityPromise_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPromiseCapabilityResolve(TNode<PromiseCapability> p_o) {
return LoadPromiseCapabilityResolve_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseCapabilityResolve(TNode<PromiseCapability> p_o, TNode<Object> p_v) {
return StorePromiseCapabilityResolve_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPromiseCapabilityReject(TNode<PromiseCapability> p_o) {
return LoadPromiseCapabilityReject_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseCapabilityReject(TNode<PromiseCapability> p_o, TNode<Object> p_v) {
return StorePromiseCapabilityReject_0(state_, p_o, p_v);}

TNode<JSArrayBuffer> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayBufferViewBuffer(TNode<JSArrayBufferView> p_o) {
return LoadJSArrayBufferViewBuffer_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayBufferViewBuffer(TNode<JSArrayBufferView> p_o, TNode<JSArrayBuffer> p_v) {
return StoreJSArrayBufferViewBuffer_0(state_, p_o, p_v);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayBufferViewByteOffset(TNode<JSArrayBufferView> p_o) {
return LoadJSArrayBufferViewByteOffset_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayBufferViewByteOffset(TNode<JSArrayBufferView> p_o, TNode<UintPtrT> p_v) {
return StoreJSArrayBufferViewByteOffset_0(state_, p_o, p_v);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayBufferViewByteLength(TNode<JSArrayBufferView> p_o) {
return LoadJSArrayBufferViewByteLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayBufferViewByteLength(TNode<JSArrayBufferView> p_o, TNode<UintPtrT> p_v) {
return StoreJSArrayBufferViewByteLength_0(state_, p_o, p_v);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::LoadJSTypedArrayLength(TNode<JSTypedArray> p_o) {
return LoadJSTypedArrayLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSTypedArrayLength(TNode<JSTypedArray> p_o, TNode<UintPtrT> p_v) {
return StoreJSTypedArrayLength_0(state_, p_o, p_v);}

TNode<ExternalPointerT> TorqueGeneratedExportedMacrosAssembler::LoadJSTypedArrayExternalPointer(TNode<JSTypedArray> p_o) {
return LoadJSTypedArrayExternalPointer_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSTypedArrayExternalPointer(TNode<JSTypedArray> p_o, TNode<ExternalPointerT> p_v) {
return StoreJSTypedArrayExternalPointer_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSTypedArrayBasePointer(TNode<JSTypedArray> p_o) {
return LoadJSTypedArrayBasePointer_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSTypedArrayBasePointer(TNode<JSTypedArray> p_o, TNode<Object> p_v) {
return StoreJSTypedArrayBasePointer_0(state_, p_o, p_v);}

TNode<Uint32T> TorqueGeneratedExportedMacrosAssembler::LoadJSTypedArrayBitField(TNode<JSTypedArray> p_o) {
return LoadJSTypedArrayBitField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSTypedArrayBitField(TNode<JSTypedArray> p_o, TNode<Uint32T> p_v) {
return StoreJSTypedArrayBitField_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadDataHandlerSmiHandler(TNode<DataHandler> p_o) {
return LoadDataHandlerSmiHandler_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDataHandlerSmiHandler(TNode<DataHandler> p_o, TNode<Object> p_v) {
return StoreDataHandlerSmiHandler_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadDataHandlerValidityCell(TNode<DataHandler> p_o) {
return LoadDataHandlerValidityCell_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDataHandlerValidityCell(TNode<DataHandler> p_o, TNode<Object> p_v) {
return StoreDataHandlerValidityCell_0(state_, p_o, p_v);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadDataHandlerData1(TNode<DataHandler> p_o) {
return LoadDataHandlerData1_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDataHandlerData1(TNode<DataHandler> p_o, TNode<MaybeObject> p_v) {
return StoreDataHandlerData1_0(state_, p_o, p_v);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadDataHandlerData2(TNode<DataHandler> p_o) {
return LoadDataHandlerData2_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDataHandlerData2(TNode<DataHandler> p_o, TNode<MaybeObject> p_v) {
return StoreDataHandlerData2_0(state_, p_o, p_v);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadDataHandlerData3(TNode<DataHandler> p_o) {
return LoadDataHandlerData3_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDataHandlerData3(TNode<DataHandler> p_o, TNode<MaybeObject> p_v) {
return StoreDataHandlerData3_0(state_, p_o, p_v);}

TNode<AllocationSite> TorqueGeneratedExportedMacrosAssembler::LoadAllocationMementoAllocationSite(TNode<AllocationMemento> p_o) {
return LoadAllocationMementoAllocationSite_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAllocationMementoAllocationSite(TNode<AllocationMemento> p_o, TNode<AllocationSite> p_v) {
return StoreAllocationMementoAllocationSite_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadCallHandlerInfoCallback(TNode<CallHandlerInfo> p_o) {
return LoadCallHandlerInfoCallback_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCallHandlerInfoCallback(TNode<CallHandlerInfo> p_o, TNode<Object> p_v) {
return StoreCallHandlerInfoCallback_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadCallHandlerInfoJsCallback(TNode<CallHandlerInfo> p_o) {
return LoadCallHandlerInfoJsCallback_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCallHandlerInfoJsCallback(TNode<CallHandlerInfo> p_o, TNode<Object> p_v) {
return StoreCallHandlerInfoJsCallback_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadCallHandlerInfoData(TNode<CallHandlerInfo> p_o) {
return LoadCallHandlerInfoData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCallHandlerInfoData(TNode<CallHandlerInfo> p_o, TNode<Object> p_v) {
return StoreCallHandlerInfoData_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadInterceptorInfoGetter(TNode<InterceptorInfo> p_o) {
return LoadInterceptorInfoGetter_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterceptorInfoGetter(TNode<InterceptorInfo> p_o, TNode<Object> p_v) {
return StoreInterceptorInfoGetter_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadInterceptorInfoSetter(TNode<InterceptorInfo> p_o) {
return LoadInterceptorInfoSetter_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterceptorInfoSetter(TNode<InterceptorInfo> p_o, TNode<Object> p_v) {
return StoreInterceptorInfoSetter_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadInterceptorInfoQuery(TNode<InterceptorInfo> p_o) {
return LoadInterceptorInfoQuery_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterceptorInfoQuery(TNode<InterceptorInfo> p_o, TNode<Object> p_v) {
return StoreInterceptorInfoQuery_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadInterceptorInfoDescriptor(TNode<InterceptorInfo> p_o) {
return LoadInterceptorInfoDescriptor_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterceptorInfoDescriptor(TNode<InterceptorInfo> p_o, TNode<Object> p_v) {
return StoreInterceptorInfoDescriptor_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadInterceptorInfoDeleter(TNode<InterceptorInfo> p_o) {
return LoadInterceptorInfoDeleter_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterceptorInfoDeleter(TNode<InterceptorInfo> p_o, TNode<Object> p_v) {
return StoreInterceptorInfoDeleter_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadInterceptorInfoEnumerator(TNode<InterceptorInfo> p_o) {
return LoadInterceptorInfoEnumerator_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterceptorInfoEnumerator(TNode<InterceptorInfo> p_o, TNode<Object> p_v) {
return StoreInterceptorInfoEnumerator_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadInterceptorInfoDefiner(TNode<InterceptorInfo> p_o) {
return LoadInterceptorInfoDefiner_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterceptorInfoDefiner(TNode<InterceptorInfo> p_o, TNode<Object> p_v) {
return StoreInterceptorInfoDefiner_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadInterceptorInfoData(TNode<InterceptorInfo> p_o) {
return LoadInterceptorInfoData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterceptorInfoData(TNode<InterceptorInfo> p_o, TNode<Object> p_v) {
return StoreInterceptorInfoData_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadInterceptorInfoFlags(TNode<InterceptorInfo> p_o) {
return LoadInterceptorInfoFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterceptorInfoFlags(TNode<InterceptorInfo> p_o, TNode<Smi> p_v) {
return StoreInterceptorInfoFlags_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessCheckInfoCallback(TNode<AccessCheckInfo> p_o) {
return LoadAccessCheckInfoCallback_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessCheckInfoCallback(TNode<AccessCheckInfo> p_o, TNode<Object> p_v) {
return StoreAccessCheckInfoCallback_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessCheckInfoNamedInterceptor(TNode<AccessCheckInfo> p_o) {
return LoadAccessCheckInfoNamedInterceptor_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessCheckInfoNamedInterceptor(TNode<AccessCheckInfo> p_o, TNode<Object> p_v) {
return StoreAccessCheckInfoNamedInterceptor_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessCheckInfoIndexedInterceptor(TNode<AccessCheckInfo> p_o) {
return LoadAccessCheckInfoIndexedInterceptor_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessCheckInfoIndexedInterceptor(TNode<AccessCheckInfo> p_o, TNode<Object> p_v) {
return StoreAccessCheckInfoIndexedInterceptor_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessCheckInfoData(TNode<AccessCheckInfo> p_o) {
return LoadAccessCheckInfoData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessCheckInfoData(TNode<AccessCheckInfo> p_o, TNode<Object> p_v) {
return StoreAccessCheckInfoData_0(state_, p_o, p_v);}

TNode<Name> TorqueGeneratedExportedMacrosAssembler::LoadAccessorInfoName(TNode<AccessorInfo> p_o) {
return LoadAccessorInfoName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessorInfoName(TNode<AccessorInfo> p_o, TNode<Name> p_v) {
return StoreAccessorInfoName_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadAccessorInfoFlags(TNode<AccessorInfo> p_o) {
return LoadAccessorInfoFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessorInfoFlags(TNode<AccessorInfo> p_o, TNode<Smi> p_v) {
return StoreAccessorInfoFlags_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessorInfoExpectedReceiverType(TNode<AccessorInfo> p_o) {
return LoadAccessorInfoExpectedReceiverType_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessorInfoExpectedReceiverType(TNode<AccessorInfo> p_o, TNode<Object> p_v) {
return StoreAccessorInfoExpectedReceiverType_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessorInfoSetter(TNode<AccessorInfo> p_o) {
return LoadAccessorInfoSetter_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessorInfoSetter(TNode<AccessorInfo> p_o, TNode<Object> p_v) {
return StoreAccessorInfoSetter_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessorInfoGetter(TNode<AccessorInfo> p_o) {
return LoadAccessorInfoGetter_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessorInfoGetter(TNode<AccessorInfo> p_o, TNode<Object> p_v) {
return StoreAccessorInfoGetter_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessorInfoJsGetter(TNode<AccessorInfo> p_o) {
return LoadAccessorInfoJsGetter_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessorInfoJsGetter(TNode<AccessorInfo> p_o, TNode<Object> p_v) {
return StoreAccessorInfoJsGetter_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessorInfoData(TNode<AccessorInfo> p_o) {
return LoadAccessorInfoData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessorInfoData(TNode<AccessorInfo> p_o, TNode<Object> p_v) {
return StoreAccessorInfoData_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSSloppyArgumentsObjectLength(TNode<JSArgumentsObject> p_o) {
return LoadJSSloppyArgumentsObjectLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSSloppyArgumentsObjectLength(TNode<JSArgumentsObject> p_o, TNode<Object> p_v) {
return StoreJSSloppyArgumentsObjectLength_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSSloppyArgumentsObjectCallee(TNode<JSArgumentsObject> p_o) {
return LoadJSSloppyArgumentsObjectCallee_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSSloppyArgumentsObjectCallee(TNode<JSArgumentsObject> p_o, TNode<Object> p_v) {
return StoreJSSloppyArgumentsObjectCallee_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSStrictArgumentsObjectLength(TNode<JSArgumentsObject> p_o) {
return LoadJSStrictArgumentsObjectLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSStrictArgumentsObjectLength(TNode<JSArgumentsObject> p_o, TNode<Object> p_v) {
return StoreJSStrictArgumentsObjectLength_0(state_, p_o, p_v);}

TNode<Context> TorqueGeneratedExportedMacrosAssembler::LoadSloppyArgumentsElementsContext(TNode<SloppyArgumentsElements> p_o) {
return LoadSloppyArgumentsElementsContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSloppyArgumentsElementsContext(TNode<SloppyArgumentsElements> p_o, TNode<Context> p_v) {
return StoreSloppyArgumentsElementsContext_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadSloppyArgumentsElementsArguments(TNode<SloppyArgumentsElements> p_o) {
return LoadSloppyArgumentsElementsArguments_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSloppyArgumentsElementsArguments(TNode<SloppyArgumentsElements> p_o, TNode<FixedArray> p_v) {
return StoreSloppyArgumentsElementsArguments_0(state_, p_o, p_v);}

TorqueStructSlice_Smi_OR_TheHole_MutableReference_Smi_OR_TheHole_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSloppyArgumentsElementsMappedEntries(TNode<SloppyArgumentsElements> p_o) {
return FieldSliceSloppyArgumentsElementsMappedEntries_0(state_, p_o);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadSloppyArgumentsElementsMappedEntries(TNode<SloppyArgumentsElements> p_o, TNode<IntPtrT> p_i) {
return LoadSloppyArgumentsElementsMappedEntries_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSloppyArgumentsElementsMappedEntries(TNode<SloppyArgumentsElements> p_o, TNode<IntPtrT> p_i, TNode<Object> p_v) {
return StoreSloppyArgumentsElementsMappedEntries_0(state_, p_o, p_i, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadAliasedArgumentsEntryAliasedContextSlot(TNode<AliasedArgumentsEntry> p_o) {
return LoadAliasedArgumentsEntryAliasedContextSlot_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAliasedArgumentsEntryAliasedContextSlot(TNode<AliasedArgumentsEntry> p_o, TNode<Smi> p_v) {
return StoreAliasedArgumentsEntryAliasedContextSlot_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadCellValue(TNode<Cell> p_o) {
return LoadCellValue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCellValue(TNode<Cell> p_o, TNode<Object> p_v) {
return StoreCellValue_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadBytecodeArrayConstantPool(TNode<BytecodeArray> p_o) {
return LoadBytecodeArrayConstantPool_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBytecodeArrayConstantPool(TNode<BytecodeArray> p_o, TNode<FixedArray> p_v) {
return StoreBytecodeArrayConstantPool_0(state_, p_o, p_v);}

TNode<ByteArray> TorqueGeneratedExportedMacrosAssembler::LoadBytecodeArrayHandlerTable(TNode<BytecodeArray> p_o) {
return LoadBytecodeArrayHandlerTable_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBytecodeArrayHandlerTable(TNode<BytecodeArray> p_o, TNode<ByteArray> p_v) {
return StoreBytecodeArrayHandlerTable_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadBytecodeArraySourcePositionTable(TNode<BytecodeArray> p_o) {
return LoadBytecodeArraySourcePositionTable_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBytecodeArraySourcePositionTable(TNode<BytecodeArray> p_o, TNode<HeapObject> p_v) {
return StoreBytecodeArraySourcePositionTable_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadBytecodeArrayFrameSize(TNode<BytecodeArray> p_o) {
return LoadBytecodeArrayFrameSize_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBytecodeArrayFrameSize(TNode<BytecodeArray> p_o, TNode<Int32T> p_v) {
return StoreBytecodeArrayFrameSize_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadBytecodeArrayParameterSize(TNode<BytecodeArray> p_o) {
return LoadBytecodeArrayParameterSize_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBytecodeArrayParameterSize(TNode<BytecodeArray> p_o, TNode<Int32T> p_v) {
return StoreBytecodeArrayParameterSize_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadBytecodeArrayIncomingNewTargetOrGeneratorRegister(TNode<BytecodeArray> p_o) {
return LoadBytecodeArrayIncomingNewTargetOrGeneratorRegister_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBytecodeArrayIncomingNewTargetOrGeneratorRegister(TNode<BytecodeArray> p_o, TNode<Int32T> p_v) {
return StoreBytecodeArrayIncomingNewTargetOrGeneratorRegister_0(state_, p_o, p_v);}

TNode<Int8T> TorqueGeneratedExportedMacrosAssembler::LoadBytecodeArrayOsrNestingLevel(TNode<BytecodeArray> p_o) {
return LoadBytecodeArrayOsrNestingLevel_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBytecodeArrayOsrNestingLevel(TNode<BytecodeArray> p_o, TNode<Int8T> p_v) {
return StoreBytecodeArrayOsrNestingLevel_0(state_, p_o, p_v);}

TNode<Int8T> TorqueGeneratedExportedMacrosAssembler::LoadBytecodeArrayBytecodeAge(TNode<BytecodeArray> p_o) {
return LoadBytecodeArrayBytecodeAge_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBytecodeArrayBytecodeAge(TNode<BytecodeArray> p_o, TNode<Int8T> p_v) {
return StoreBytecodeArrayBytecodeAge_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoFlags(TNode<ScopeInfo> p_o) {
return LoadScopeInfoFlags_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoParameterCount(TNode<ScopeInfo> p_o) {
return LoadScopeInfoParameterCount_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScopeInfoParameterCount(TNode<ScopeInfo> p_o, TNode<Smi> p_v) {
return StoreScopeInfoParameterCount_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoContextLocalCount(TNode<ScopeInfo> p_o) {
return LoadScopeInfoContextLocalCount_0(state_, p_o);}

TorqueStructSlice_String_MutableReference_String_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoContextLocalNames(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoContextLocalNames_0(state_, p_o);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoContextLocalNames(TNode<ScopeInfo> p_o, TNode<IntPtrT> p_i) {
return LoadScopeInfoContextLocalNames_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreScopeInfoContextLocalNames(TNode<ScopeInfo> p_o, TNode<IntPtrT> p_i, TNode<String> p_v) {
return StoreScopeInfoContextLocalNames_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_SmiTagged_VariableProperties_MutableReference_SmiTagged_VariableProperties_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoContextLocalInfos(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoContextLocalInfos_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoContextLocalInfos(TNode<ScopeInfo> p_o, TNode<IntPtrT> p_i) {
return LoadScopeInfoContextLocalInfos_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreScopeInfoContextLocalInfos(TNode<ScopeInfo> p_o, TNode<IntPtrT> p_i, TNode<Smi> p_v) {
return StoreScopeInfoContextLocalInfos_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_Smi_MutableReference_Smi_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoSavedClassVariableInfo(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoSavedClassVariableInfo_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoSavedClassVariableInfo(TNode<ScopeInfo> p_o) {
return LoadScopeInfoSavedClassVariableInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScopeInfoSavedClassVariableInfo(TNode<ScopeInfo> p_o, TNode<Smi> p_v) {
return StoreScopeInfoSavedClassVariableInfo_0(state_, p_o, p_v);}

TorqueStructSlice_Smi_MutableReference_Smi_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoReceiverInfo(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoReceiverInfo_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoReceiverInfo(TNode<ScopeInfo> p_o) {
return LoadScopeInfoReceiverInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScopeInfoReceiverInfo(TNode<ScopeInfo> p_o, TNode<Smi> p_v) {
return StoreScopeInfoReceiverInfo_0(state_, p_o, p_v);}

TorqueStructSlice_FunctionVariableInfo_MutableReference_FunctionVariableInfo_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoFunctionVariableInfo(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoFunctionVariableInfo_0(state_, p_o);}

TorqueStructSlice_String_OR_Undefined_MutableReference_String_OR_Undefined_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoInferredFunctionName(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoInferredFunctionName_0(state_, p_o);}

TNode<PrimitiveHeapObject> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoInferredFunctionName(TNode<ScopeInfo> p_o) {
return LoadScopeInfoInferredFunctionName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScopeInfoInferredFunctionName(TNode<ScopeInfo> p_o, TNode<PrimitiveHeapObject> p_v) {
return StoreScopeInfoInferredFunctionName_0(state_, p_o, p_v);}

TorqueStructSlice_PositionInfo_MutableReference_PositionInfo_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoPositionInfo(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoPositionInfo_0(state_, p_o);}

TorqueStructSlice_TheHole_OR_ScopeInfo_MutableReference_TheHole_OR_ScopeInfo_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoOuterScopeInfo(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoOuterScopeInfo_0(state_, p_o);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoOuterScopeInfo(TNode<ScopeInfo> p_o) {
return LoadScopeInfoOuterScopeInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScopeInfoOuterScopeInfo(TNode<ScopeInfo> p_o, TNode<HeapObject> p_v) {
return StoreScopeInfoOuterScopeInfo_0(state_, p_o, p_v);}

TorqueStructSlice_HashTable_MutableReference_HashTable_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoLocalsBlockList(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoLocalsBlockList_0(state_, p_o);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoLocalsBlockList(TNode<ScopeInfo> p_o) {
return LoadScopeInfoLocalsBlockList_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScopeInfoLocalsBlockList(TNode<ScopeInfo> p_o, TNode<FixedArray> p_v) {
return StoreScopeInfoLocalsBlockList_0(state_, p_o, p_v);}

TorqueStructSlice_SourceTextModuleInfo_MutableReference_SourceTextModuleInfo_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoModuleInfo(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoModuleInfo_0(state_, p_o);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoModuleInfo(TNode<ScopeInfo> p_o) {
return LoadScopeInfoModuleInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScopeInfoModuleInfo(TNode<ScopeInfo> p_o, TNode<FixedArray> p_v) {
return StoreScopeInfoModuleInfo_0(state_, p_o, p_v);}

TorqueStructSlice_Smi_ConstReference_Smi_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoModuleVariableCount(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoModuleVariableCount_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScopeInfoModuleVariableCount(TNode<ScopeInfo> p_o) {
return LoadScopeInfoModuleVariableCount_0(state_, p_o);}

TorqueStructSlice_ModuleVariable_MutableReference_ModuleVariable_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceScopeInfoModuleVariables(TNode<ScopeInfo> p_o) {
return FieldSliceScopeInfoModuleVariables_0(state_, p_o);}

TorqueStructSlice_float64_or_hole_MutableReference_float64_or_hole_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceFixedDoubleArrayFloats(TNode<FixedDoubleArray> p_o) {
return FieldSliceFixedDoubleArrayFloats_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadBreakPointId(TNode<BreakPoint> p_o) {
return LoadBreakPointId_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBreakPointId(TNode<BreakPoint> p_o, TNode<Smi> p_v) {
return StoreBreakPointId_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadBreakPointCondition(TNode<BreakPoint> p_o) {
return LoadBreakPointCondition_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBreakPointCondition(TNode<BreakPoint> p_o, TNode<String> p_v) {
return StoreBreakPointCondition_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadBreakPointInfoSourcePosition(TNode<BreakPointInfo> p_o) {
return LoadBreakPointInfoSourcePosition_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBreakPointInfoSourcePosition(TNode<BreakPointInfo> p_o, TNode<Smi> p_v) {
return StoreBreakPointInfoSourcePosition_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadBreakPointInfoBreakPoints(TNode<BreakPointInfo> p_o) {
return LoadBreakPointInfoBreakPoints_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBreakPointInfoBreakPoints(TNode<BreakPointInfo> p_o, TNode<HeapObject> p_v) {
return StoreBreakPointInfoBreakPoints_0(state_, p_o, p_v);}

TNode<SharedFunctionInfo> TorqueGeneratedExportedMacrosAssembler::LoadDebugInfoShared(TNode<DebugInfo> p_o) {
return LoadDebugInfoShared_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDebugInfoShared(TNode<DebugInfo> p_o, TNode<SharedFunctionInfo> p_v) {
return StoreDebugInfoShared_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadDebugInfoDebuggerHints(TNode<DebugInfo> p_o) {
return LoadDebugInfoDebuggerHints_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDebugInfoDebuggerHints(TNode<DebugInfo> p_o, TNode<Smi> p_v) {
return StoreDebugInfoDebuggerHints_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadDebugInfoScript(TNode<DebugInfo> p_o) {
return LoadDebugInfoScript_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDebugInfoScript(TNode<DebugInfo> p_o, TNode<HeapObject> p_v) {
return StoreDebugInfoScript_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadDebugInfoOriginalBytecodeArray(TNode<DebugInfo> p_o) {
return LoadDebugInfoOriginalBytecodeArray_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDebugInfoOriginalBytecodeArray(TNode<DebugInfo> p_o, TNode<HeapObject> p_v) {
return StoreDebugInfoOriginalBytecodeArray_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadDebugInfoDebugBytecodeArray(TNode<DebugInfo> p_o) {
return LoadDebugInfoDebugBytecodeArray_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDebugInfoDebugBytecodeArray(TNode<DebugInfo> p_o, TNode<HeapObject> p_v) {
return StoreDebugInfoDebugBytecodeArray_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadDebugInfoBreakPoints(TNode<DebugInfo> p_o) {
return LoadDebugInfoBreakPoints_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDebugInfoBreakPoints(TNode<DebugInfo> p_o, TNode<FixedArray> p_v) {
return StoreDebugInfoBreakPoints_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadDebugInfoFlags(TNode<DebugInfo> p_o) {
return LoadDebugInfoFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDebugInfoFlags(TNode<DebugInfo> p_o, TNode<Smi> p_v) {
return StoreDebugInfoFlags_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadDebugInfoCoverageInfo(TNode<DebugInfo> p_o) {
return LoadDebugInfoCoverageInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDebugInfoCoverageInfo(TNode<DebugInfo> p_o, TNode<HeapObject> p_v) {
return StoreDebugInfoCoverageInfo_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadCoverageInfoSlotCount(TNode<CoverageInfo> p_o) {
return LoadCoverageInfoSlotCount_0(state_, p_o);}

TorqueStructSlice_CoverageInfoSlot_MutableReference_CoverageInfoSlot_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceCoverageInfoSlots(TNode<CoverageInfo> p_o) {
return FieldSliceCoverageInfoSlots_0(state_, p_o);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadEnumCacheKeys(TNode<EnumCache> p_o) {
return LoadEnumCacheKeys_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreEnumCacheKeys(TNode<EnumCache> p_o, TNode<FixedArray> p_v) {
return StoreEnumCacheKeys_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadEnumCacheIndices(TNode<EnumCache> p_o) {
return LoadEnumCacheIndices_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreEnumCacheIndices(TNode<EnumCache> p_o, TNode<FixedArray> p_v) {
return StoreEnumCacheIndices_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessorPairGetter(TNode<AccessorPair> p_o) {
return LoadAccessorPairGetter_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessorPairGetter(TNode<AccessorPair> p_o, TNode<Object> p_v) {
return StoreAccessorPairGetter_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAccessorPairSetter(TNode<AccessorPair> p_o) {
return LoadAccessorPairSetter_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAccessorPairSetter(TNode<AccessorPair> p_o, TNode<Object> p_v) {
return StoreAccessorPairSetter_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadClassPositionsStart(TNode<ClassPositions> p_o) {
return LoadClassPositionsStart_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreClassPositionsStart(TNode<ClassPositions> p_o, TNode<Smi> p_v) {
return StoreClassPositionsStart_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadClassPositionsEnd(TNode<ClassPositions> p_o) {
return LoadClassPositionsEnd_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreClassPositionsEnd(TNode<ClassPositions> p_o, TNode<Smi> p_v) {
return StoreClassPositionsEnd_0(state_, p_o, p_v);}

TNode<Uint16T> TorqueGeneratedExportedMacrosAssembler::LoadDescriptorArrayNumberOfAllDescriptors(TNode<DescriptorArray> p_o) {
return LoadDescriptorArrayNumberOfAllDescriptors_0(state_, p_o);}

TNode<Uint16T> TorqueGeneratedExportedMacrosAssembler::LoadDescriptorArrayNumberOfDescriptors(TNode<DescriptorArray> p_o) {
return LoadDescriptorArrayNumberOfDescriptors_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDescriptorArrayNumberOfDescriptors(TNode<DescriptorArray> p_o, TNode<Uint16T> p_v) {
return StoreDescriptorArrayNumberOfDescriptors_0(state_, p_o, p_v);}

TNode<Uint16T> TorqueGeneratedExportedMacrosAssembler::LoadDescriptorArrayRawNumberOfMarkedDescriptors(TNode<DescriptorArray> p_o) {
return LoadDescriptorArrayRawNumberOfMarkedDescriptors_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDescriptorArrayRawNumberOfMarkedDescriptors(TNode<DescriptorArray> p_o, TNode<Uint16T> p_v) {
return StoreDescriptorArrayRawNumberOfMarkedDescriptors_0(state_, p_o, p_v);}

TNode<Uint16T> TorqueGeneratedExportedMacrosAssembler::LoadDescriptorArrayFiller16Bits(TNode<DescriptorArray> p_o) {
return LoadDescriptorArrayFiller16Bits_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDescriptorArrayFiller16Bits(TNode<DescriptorArray> p_o, TNode<Uint16T> p_v) {
return StoreDescriptorArrayFiller16Bits_0(state_, p_o, p_v);}

TNode<EnumCache> TorqueGeneratedExportedMacrosAssembler::LoadDescriptorArrayEnumCache(TNode<DescriptorArray> p_o) {
return LoadDescriptorArrayEnumCache_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreDescriptorArrayEnumCache(TNode<DescriptorArray> p_o, TNode<EnumCache> p_v) {
return StoreDescriptorArrayEnumCache_0(state_, p_o, p_v);}

TorqueStructSlice_DescriptorEntry_MutableReference_DescriptorEntry_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceDescriptorArrayDescriptors(TNode<DescriptorArray> p_o) {
return FieldSliceDescriptorArrayDescriptors_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadEmbedderDataArrayLength(TNode<EmbedderDataArray> p_o) {
return LoadEmbedderDataArrayLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreEmbedderDataArrayLength(TNode<EmbedderDataArray> p_o, TNode<Smi> p_v) {
return StoreEmbedderDataArrayLength_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackCellValue(TNode<FeedbackCell> p_o) {
return LoadFeedbackCellValue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFeedbackCellValue(TNode<FeedbackCell> p_o, TNode<HeapObject> p_v) {
return StoreFeedbackCellValue_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackCellInterruptBudget(TNode<FeedbackCell> p_o) {
return LoadFeedbackCellInterruptBudget_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFeedbackCellInterruptBudget(TNode<FeedbackCell> p_o, TNode<Int32T> p_v) {
return StoreFeedbackCellInterruptBudget_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackVectorLength(TNode<FeedbackVector> p_o) {
return LoadFeedbackVectorLength_0(state_, p_o);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackVectorInvocationCount(TNode<FeedbackVector> p_o) {
return LoadFeedbackVectorInvocationCount_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFeedbackVectorInvocationCount(TNode<FeedbackVector> p_o, TNode<Int32T> p_v) {
return StoreFeedbackVectorInvocationCount_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackVectorProfilerTicks(TNode<FeedbackVector> p_o) {
return LoadFeedbackVectorProfilerTicks_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFeedbackVectorProfilerTicks(TNode<FeedbackVector> p_o, TNode<Int32T> p_v) {
return StoreFeedbackVectorProfilerTicks_0(state_, p_o, p_v);}

TNode<Uint32T> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackVectorFlags(TNode<FeedbackVector> p_o) {
return LoadFeedbackVectorFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFeedbackVectorFlags(TNode<FeedbackVector> p_o, TNode<Uint32T> p_v) {
return StoreFeedbackVectorFlags_0(state_, p_o, p_v);}

TNode<SharedFunctionInfo> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackVectorSharedFunctionInfo(TNode<FeedbackVector> p_o) {
return LoadFeedbackVectorSharedFunctionInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFeedbackVectorSharedFunctionInfo(TNode<FeedbackVector> p_o, TNode<SharedFunctionInfo> p_v) {
return StoreFeedbackVectorSharedFunctionInfo_0(state_, p_o, p_v);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackVectorMaybeOptimizedCode(TNode<FeedbackVector> p_o) {
return LoadFeedbackVectorMaybeOptimizedCode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFeedbackVectorMaybeOptimizedCode(TNode<FeedbackVector> p_o, TNode<MaybeObject> p_v) {
return StoreFeedbackVectorMaybeOptimizedCode_0(state_, p_o, p_v);}

TNode<ClosureFeedbackCellArray> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackVectorClosureFeedbackCellArray(TNode<FeedbackVector> p_o) {
return LoadFeedbackVectorClosureFeedbackCellArray_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFeedbackVectorClosureFeedbackCellArray(TNode<FeedbackVector> p_o, TNode<ClosureFeedbackCellArray> p_v) {
return StoreFeedbackVectorClosureFeedbackCellArray_0(state_, p_o, p_v);}

TorqueStructSlice_MaybeObject_MutableReference_MaybeObject_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceFeedbackVectorRawFeedbackSlots(TNode<FeedbackVector> p_o) {
return FieldSliceFeedbackVectorRawFeedbackSlots_0(state_, p_o);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadFeedbackVectorRawFeedbackSlots(TNode<FeedbackVector> p_o, TNode<IntPtrT> p_i) {
return LoadFeedbackVectorRawFeedbackSlots_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreFeedbackVectorRawFeedbackSlots(TNode<FeedbackVector> p_o, TNode<IntPtrT> p_i, TNode<MaybeObject> p_v) {
return StoreFeedbackVectorRawFeedbackSlots_0(state_, p_o, p_i, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadWeakArrayListCapacity(TNode<WeakArrayList> p_o) {
return LoadWeakArrayListCapacity_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadWeakArrayListLength(TNode<WeakArrayList> p_o) {
return LoadWeakArrayListLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakArrayListLength(TNode<WeakArrayList> p_o, TNode<Smi> p_v) {
return StoreWeakArrayListLength_0(state_, p_o, p_v);}

TorqueStructSlice_MaybeObject_MutableReference_MaybeObject_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceWeakArrayListObjects(TNode<WeakArrayList> p_o) {
return FieldSliceWeakArrayListObjects_0(state_, p_o);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadWeakArrayListObjects(TNode<WeakArrayList> p_o, TNode<IntPtrT> p_i) {
return LoadWeakArrayListObjects_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreWeakArrayListObjects(TNode<WeakArrayList> p_o, TNode<IntPtrT> p_i, TNode<MaybeObject> p_v) {
return StoreWeakArrayListObjects_0(state_, p_o, p_i, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadFreeSpaceSize(TNode<FreeSpace> p_o) {
return LoadFreeSpaceSize_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFreeSpaceSize(TNode<FreeSpace> p_o, TNode<Smi> p_v) {
return StoreFreeSpaceSize_0(state_, p_o, p_v);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadFreeSpaceNext(TNode<FreeSpace> p_o) {
return LoadFreeSpaceNext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFreeSpaceNext(TNode<FreeSpace> p_o, TNode<MaybeObject> p_v) {
return StoreFreeSpaceNext_0(state_, p_o, p_v);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayBufferByteLength(TNode<JSArrayBuffer> p_o) {
return LoadJSArrayBufferByteLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayBufferByteLength(TNode<JSArrayBuffer> p_o, TNode<UintPtrT> p_v) {
return StoreJSArrayBufferByteLength_0(state_, p_o, p_v);}

TNode<UintPtrT> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayBufferMaxByteLength(TNode<JSArrayBuffer> p_o) {
return LoadJSArrayBufferMaxByteLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayBufferMaxByteLength(TNode<JSArrayBuffer> p_o, TNode<UintPtrT> p_v) {
return StoreJSArrayBufferMaxByteLength_0(state_, p_o, p_v);}

TNode<ExternalPointerT> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayBufferBackingStore(TNode<JSArrayBuffer> p_o) {
return LoadJSArrayBufferBackingStore_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayBufferBackingStore(TNode<JSArrayBuffer> p_o, TNode<ExternalPointerT> p_v) {
return StoreJSArrayBufferBackingStore_0(state_, p_o, p_v);}

TNode<RawPtrT> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayBufferExtension(TNode<JSArrayBuffer> p_o) {
return LoadJSArrayBufferExtension_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayBufferExtension(TNode<JSArrayBuffer> p_o, TNode<RawPtrT> p_v) {
return StoreJSArrayBufferExtension_0(state_, p_o, p_v);}

TNode<Uint32T> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayBufferBitField(TNode<JSArrayBuffer> p_o) {
return LoadJSArrayBufferBitField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayBufferBitField(TNode<JSArrayBuffer> p_o, TNode<Uint32T> p_v) {
return StoreJSArrayBufferBitField_0(state_, p_o, p_v);}

TNode<ExternalPointerT> TorqueGeneratedExportedMacrosAssembler::LoadJSDataViewDataPointer(TNode<JSDataView> p_o) {
return LoadJSDataViewDataPointer_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDataViewDataPointer(TNode<JSDataView> p_o, TNode<ExternalPointerT> p_v) {
return StoreJSDataViewDataPointer_0(state_, p_o, p_v);}

TNode<JSReceiver> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayIteratorIteratedObject(TNode<JSArrayIterator> p_o) {
return LoadJSArrayIteratorIteratedObject_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayIteratorIteratedObject(TNode<JSArrayIterator> p_o, TNode<JSReceiver> p_v) {
return StoreJSArrayIteratorIteratedObject_0(state_, p_o, p_v);}

TNode<Number> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayIteratorNextIndex(TNode<JSArrayIterator> p_o) {
return LoadJSArrayIteratorNextIndex_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayIteratorNextIndex(TNode<JSArrayIterator> p_o, TNode<Number> p_v) {
return StoreJSArrayIteratorNextIndex_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayIteratorKind(TNode<JSArrayIterator> p_o) {
return LoadJSArrayIteratorKind_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayIteratorKind(TNode<JSArrayIterator> p_o, TNode<Smi> p_v) {
return StoreJSArrayIteratorKind_0(state_, p_o, p_v);}

TNode<Number> TorqueGeneratedExportedMacrosAssembler::LoadJSArrayLength(TNode<JSArray> p_o) {
return LoadJSArrayLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSArrayLength(TNode<JSArray> p_o, TNode<Number> p_v) {
return StoreJSArrayLength_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSCollectionIteratorTable(TNode<JSCollectionIterator> p_o) {
return LoadJSCollectionIteratorTable_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSCollectionIteratorTable(TNode<JSCollectionIterator> p_o, TNode<Object> p_v) {
return StoreJSCollectionIteratorTable_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSCollectionIteratorIndex(TNode<JSCollectionIterator> p_o) {
return LoadJSCollectionIteratorIndex_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSCollectionIteratorIndex(TNode<JSCollectionIterator> p_o, TNode<Object> p_v) {
return StoreJSCollectionIteratorIndex_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSCollectionTable(TNode<JSCollection> p_o) {
return LoadJSCollectionTable_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSCollectionTable(TNode<JSCollection> p_o, TNode<Object> p_v) {
return StoreJSCollectionTable_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSWeakCollectionTable(TNode<JSWeakCollection> p_o) {
return LoadJSWeakCollectionTable_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSWeakCollectionTable(TNode<JSWeakCollection> p_o, TNode<Object> p_v) {
return StoreJSWeakCollectionTable_0(state_, p_o, p_v);}

TNode<JSFunction> TorqueGeneratedExportedMacrosAssembler::LoadJSGeneratorObjectFunction(TNode<JSGeneratorObject> p_o) {
return LoadJSGeneratorObjectFunction_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGeneratorObjectFunction(TNode<JSGeneratorObject> p_o, TNode<JSFunction> p_v) {
return StoreJSGeneratorObjectFunction_0(state_, p_o, p_v);}

TNode<Context> TorqueGeneratedExportedMacrosAssembler::LoadJSGeneratorObjectContext(TNode<JSGeneratorObject> p_o) {
return LoadJSGeneratorObjectContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGeneratorObjectContext(TNode<JSGeneratorObject> p_o, TNode<Context> p_v) {
return StoreJSGeneratorObjectContext_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSGeneratorObjectReceiver(TNode<JSGeneratorObject> p_o) {
return LoadJSGeneratorObjectReceiver_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGeneratorObjectReceiver(TNode<JSGeneratorObject> p_o, TNode<Object> p_v) {
return StoreJSGeneratorObjectReceiver_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSGeneratorObjectInputOrDebugPos(TNode<JSGeneratorObject> p_o) {
return LoadJSGeneratorObjectInputOrDebugPos_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGeneratorObjectInputOrDebugPos(TNode<JSGeneratorObject> p_o, TNode<Object> p_v) {
return StoreJSGeneratorObjectInputOrDebugPos_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSGeneratorObjectResumeMode(TNode<JSGeneratorObject> p_o) {
return LoadJSGeneratorObjectResumeMode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGeneratorObjectResumeMode(TNode<JSGeneratorObject> p_o, TNode<Smi> p_v) {
return StoreJSGeneratorObjectResumeMode_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSGeneratorObjectContinuation(TNode<JSGeneratorObject> p_o) {
return LoadJSGeneratorObjectContinuation_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGeneratorObjectContinuation(TNode<JSGeneratorObject> p_o, TNode<Smi> p_v) {
return StoreJSGeneratorObjectContinuation_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadJSGeneratorObjectParametersAndRegisters(TNode<JSGeneratorObject> p_o) {
return LoadJSGeneratorObjectParametersAndRegisters_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGeneratorObjectParametersAndRegisters(TNode<JSGeneratorObject> p_o, TNode<FixedArray> p_v) {
return StoreJSGeneratorObjectParametersAndRegisters_0(state_, p_o, p_v);}

TNode<JSPromise> TorqueGeneratedExportedMacrosAssembler::LoadJSAsyncFunctionObjectPromise(TNode<JSAsyncFunctionObject> p_o) {
return LoadJSAsyncFunctionObjectPromise_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSAsyncFunctionObjectPromise(TNode<JSAsyncFunctionObject> p_o, TNode<JSPromise> p_v) {
return StoreJSAsyncFunctionObjectPromise_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSAsyncGeneratorObjectQueue(TNode<JSAsyncGeneratorObject> p_o) {
return LoadJSAsyncGeneratorObjectQueue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSAsyncGeneratorObjectQueue(TNode<JSAsyncGeneratorObject> p_o, TNode<HeapObject> p_v) {
return StoreJSAsyncGeneratorObjectQueue_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSAsyncGeneratorObjectIsAwaiting(TNode<JSAsyncGeneratorObject> p_o) {
return LoadJSAsyncGeneratorObjectIsAwaiting_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSAsyncGeneratorObjectIsAwaiting(TNode<JSAsyncGeneratorObject> p_o, TNode<Smi> p_v) {
return StoreJSAsyncGeneratorObjectIsAwaiting_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadAsyncGeneratorRequestNext(TNode<AsyncGeneratorRequest> p_o) {
return LoadAsyncGeneratorRequestNext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAsyncGeneratorRequestNext(TNode<AsyncGeneratorRequest> p_o, TNode<HeapObject> p_v) {
return StoreAsyncGeneratorRequestNext_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadAsyncGeneratorRequestResumeMode(TNode<AsyncGeneratorRequest> p_o) {
return LoadAsyncGeneratorRequestResumeMode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAsyncGeneratorRequestResumeMode(TNode<AsyncGeneratorRequest> p_o, TNode<Smi> p_v) {
return StoreAsyncGeneratorRequestResumeMode_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadAsyncGeneratorRequestValue(TNode<AsyncGeneratorRequest> p_o) {
return LoadAsyncGeneratorRequestValue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAsyncGeneratorRequestValue(TNode<AsyncGeneratorRequest> p_o, TNode<Object> p_v) {
return StoreAsyncGeneratorRequestValue_0(state_, p_o, p_v);}

TNode<JSPromise> TorqueGeneratedExportedMacrosAssembler::LoadAsyncGeneratorRequestPromise(TNode<AsyncGeneratorRequest> p_o) {
return LoadAsyncGeneratorRequestPromise_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreAsyncGeneratorRequestPromise(TNode<AsyncGeneratorRequest> p_o, TNode<JSPromise> p_v) {
return StoreAsyncGeneratorRequestPromise_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSGlobalProxyNativeContext(TNode<JSGlobalProxy> p_o) {
return LoadJSGlobalProxyNativeContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGlobalProxyNativeContext(TNode<JSGlobalProxy> p_o, TNode<Object> p_v) {
return StoreJSGlobalProxyNativeContext_0(state_, p_o, p_v);}

TNode<NativeContext> TorqueGeneratedExportedMacrosAssembler::LoadJSGlobalObjectNativeContext(TNode<JSGlobalObject> p_o) {
return LoadJSGlobalObjectNativeContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGlobalObjectNativeContext(TNode<JSGlobalObject> p_o, TNode<NativeContext> p_v) {
return StoreJSGlobalObjectNativeContext_0(state_, p_o, p_v);}

TNode<JSGlobalProxy> TorqueGeneratedExportedMacrosAssembler::LoadJSGlobalObjectGlobalProxy(TNode<JSGlobalObject> p_o) {
return LoadJSGlobalObjectGlobalProxy_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSGlobalObjectGlobalProxy(TNode<JSGlobalObject> p_o, TNode<JSGlobalProxy> p_v) {
return StoreJSGlobalObjectGlobalProxy_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSPrimitiveWrapperValue(TNode<JSPrimitiveWrapper> p_o) {
return LoadJSPrimitiveWrapperValue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSPrimitiveWrapperValue(TNode<JSPrimitiveWrapper> p_o, TNode<Object> p_v) {
return StoreJSPrimitiveWrapperValue_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSMessageObjectMessageType(TNode<JSMessageObject> p_o) {
return LoadJSMessageObjectMessageType_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSMessageObjectMessageType(TNode<JSMessageObject> p_o, TNode<Smi> p_v) {
return StoreJSMessageObjectMessageType_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSMessageObjectArguments(TNode<JSMessageObject> p_o) {
return LoadJSMessageObjectArguments_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSMessageObjectArguments(TNode<JSMessageObject> p_o, TNode<Object> p_v) {
return StoreJSMessageObjectArguments_0(state_, p_o, p_v);}

TNode<Script> TorqueGeneratedExportedMacrosAssembler::LoadJSMessageObjectScript(TNode<JSMessageObject> p_o) {
return LoadJSMessageObjectScript_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSMessageObjectScript(TNode<JSMessageObject> p_o, TNode<Script> p_v) {
return StoreJSMessageObjectScript_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSMessageObjectStackFrames(TNode<JSMessageObject> p_o) {
return LoadJSMessageObjectStackFrames_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSMessageObjectStackFrames(TNode<JSMessageObject> p_o, TNode<Object> p_v) {
return StoreJSMessageObjectStackFrames_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSMessageObjectSharedInfo(TNode<JSMessageObject> p_o) {
return LoadJSMessageObjectSharedInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSMessageObjectSharedInfo(TNode<JSMessageObject> p_o, TNode<HeapObject> p_v) {
return StoreJSMessageObjectSharedInfo_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSMessageObjectBytecodeOffset(TNode<JSMessageObject> p_o) {
return LoadJSMessageObjectBytecodeOffset_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSMessageObjectBytecodeOffset(TNode<JSMessageObject> p_o, TNode<Smi> p_v) {
return StoreJSMessageObjectBytecodeOffset_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSMessageObjectStartPosition(TNode<JSMessageObject> p_o) {
return LoadJSMessageObjectStartPosition_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSMessageObjectStartPosition(TNode<JSMessageObject> p_o, TNode<Smi> p_v) {
return StoreJSMessageObjectStartPosition_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSMessageObjectEndPosition(TNode<JSMessageObject> p_o) {
return LoadJSMessageObjectEndPosition_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSMessageObjectEndPosition(TNode<JSMessageObject> p_o, TNode<Smi> p_v) {
return StoreJSMessageObjectEndPosition_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSMessageObjectErrorLevel(TNode<JSMessageObject> p_o) {
return LoadJSMessageObjectErrorLevel_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSMessageObjectErrorLevel(TNode<JSMessageObject> p_o, TNode<Smi> p_v) {
return StoreJSMessageObjectErrorLevel_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSDateValue(TNode<JSDate> p_o) {
return LoadJSDateValue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDateValue(TNode<JSDate> p_o, TNode<Object> p_v) {
return StoreJSDateValue_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSDateYear(TNode<JSDate> p_o) {
return LoadJSDateYear_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDateYear(TNode<JSDate> p_o, TNode<Object> p_v) {
return StoreJSDateYear_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSDateMonth(TNode<JSDate> p_o) {
return LoadJSDateMonth_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDateMonth(TNode<JSDate> p_o, TNode<Object> p_v) {
return StoreJSDateMonth_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSDateDay(TNode<JSDate> p_o) {
return LoadJSDateDay_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDateDay(TNode<JSDate> p_o, TNode<Object> p_v) {
return StoreJSDateDay_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSDateWeekday(TNode<JSDate> p_o) {
return LoadJSDateWeekday_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDateWeekday(TNode<JSDate> p_o, TNode<Object> p_v) {
return StoreJSDateWeekday_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSDateHour(TNode<JSDate> p_o) {
return LoadJSDateHour_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDateHour(TNode<JSDate> p_o, TNode<Object> p_v) {
return StoreJSDateHour_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSDateMin(TNode<JSDate> p_o) {
return LoadJSDateMin_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDateMin(TNode<JSDate> p_o, TNode<Object> p_v) {
return StoreJSDateMin_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSDateSec(TNode<JSDate> p_o) {
return LoadJSDateSec_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDateSec(TNode<JSDate> p_o, TNode<Object> p_v) {
return StoreJSDateSec_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSDateCacheStamp(TNode<JSDate> p_o) {
return LoadJSDateCacheStamp_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSDateCacheStamp(TNode<JSDate> p_o, TNode<Object> p_v) {
return StoreJSDateCacheStamp_0(state_, p_o, p_v);}

TNode<JSReceiver> TorqueGeneratedExportedMacrosAssembler::LoadJSAsyncFromSyncIteratorSyncIterator(TNode<JSAsyncFromSyncIterator> p_o) {
return LoadJSAsyncFromSyncIteratorSyncIterator_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSAsyncFromSyncIteratorSyncIterator(TNode<JSAsyncFromSyncIterator> p_o, TNode<JSReceiver> p_v) {
return StoreJSAsyncFromSyncIteratorSyncIterator_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSAsyncFromSyncIteratorNext(TNode<JSAsyncFromSyncIterator> p_o) {
return LoadJSAsyncFromSyncIteratorNext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSAsyncFromSyncIteratorNext(TNode<JSAsyncFromSyncIterator> p_o, TNode<Object> p_v) {
return StoreJSAsyncFromSyncIteratorNext_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadJSStringIteratorString(TNode<JSStringIterator> p_o) {
return LoadJSStringIteratorString_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSStringIteratorString(TNode<JSStringIterator> p_o, TNode<String> p_v) {
return StoreJSStringIteratorString_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSStringIteratorIndex(TNode<JSStringIterator> p_o) {
return LoadJSStringIteratorIndex_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSStringIteratorIndex(TNode<JSStringIterator> p_o, TNode<Smi> p_v) {
return StoreJSStringIteratorIndex_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSProxyRevocableResultProxy(TNode<JSObject> p_o) {
return LoadJSProxyRevocableResultProxy_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSProxyRevocableResultProxy(TNode<JSObject> p_o, TNode<Object> p_v) {
return StoreJSProxyRevocableResultProxy_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSProxyRevocableResultRevoke(TNode<JSObject> p_o) {
return LoadJSProxyRevocableResultRevoke_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSProxyRevocableResultRevoke(TNode<JSObject> p_o, TNode<Object> p_v) {
return StoreJSProxyRevocableResultRevoke_0(state_, p_o, p_v);}

TNode<JSReceiver> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpStringIteratorIteratingRegExp(TNode<JSRegExpStringIterator> p_o) {
return LoadJSRegExpStringIteratorIteratingRegExp_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpStringIteratorIteratingRegExp(TNode<JSRegExpStringIterator> p_o, TNode<JSReceiver> p_v) {
return StoreJSRegExpStringIteratorIteratingRegExp_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpStringIteratorIteratedString(TNode<JSRegExpStringIterator> p_o) {
return LoadJSRegExpStringIteratorIteratedString_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpStringIteratorIteratedString(TNode<JSRegExpStringIterator> p_o, TNode<String> p_v) {
return StoreJSRegExpStringIteratorIteratedString_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpStringIteratorFlags(TNode<JSRegExpStringIterator> p_o) {
return LoadJSRegExpStringIteratorFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpStringIteratorFlags(TNode<JSRegExpStringIterator> p_o, TNode<Smi> p_v) {
return StoreJSRegExpStringIteratorFlags_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpData(TNode<JSRegExp> p_o) {
return LoadJSRegExpData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpData(TNode<JSRegExp> p_o, TNode<HeapObject> p_v) {
return StoreJSRegExpData_0(state_, p_o, p_v);}

TNode<PrimitiveHeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpSource(TNode<JSRegExp> p_o) {
return LoadJSRegExpSource_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpSource(TNode<JSRegExp> p_o, TNode<PrimitiveHeapObject> p_v) {
return StoreJSRegExpSource_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpFlags(TNode<JSRegExp> p_o) {
return LoadJSRegExpFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpFlags(TNode<JSRegExp> p_o, TNode<Object> p_v) {
return StoreJSRegExpFlags_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpResultIndex(TNode<JSArray> p_o) {
return LoadJSRegExpResultIndex_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpResultIndex(TNode<JSArray> p_o, TNode<Object> p_v) {
return StoreJSRegExpResultIndex_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpResultInput(TNode<JSArray> p_o) {
return LoadJSRegExpResultInput_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpResultInput(TNode<JSArray> p_o, TNode<Object> p_v) {
return StoreJSRegExpResultInput_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpResultGroups(TNode<JSArray> p_o) {
return LoadJSRegExpResultGroups_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpResultGroups(TNode<JSArray> p_o, TNode<Object> p_v) {
return StoreJSRegExpResultGroups_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpResultNames(TNode<JSArray> p_o) {
return LoadJSRegExpResultNames_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpResultNames(TNode<JSArray> p_o, TNode<HeapObject> p_v) {
return StoreJSRegExpResultNames_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpResultRegexpInput(TNode<JSArray> p_o) {
return LoadJSRegExpResultRegexpInput_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpResultRegexpInput(TNode<JSArray> p_o, TNode<String> p_v) {
return StoreJSRegExpResultRegexpInput_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpResultRegexpLastIndex(TNode<JSArray> p_o) {
return LoadJSRegExpResultRegexpLastIndex_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpResultRegexpLastIndex(TNode<JSArray> p_o, TNode<Smi> p_v) {
return StoreJSRegExpResultRegexpLastIndex_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpResultWithIndicesIndices(TNode<JSRegExpResult> p_o) {
return LoadJSRegExpResultWithIndicesIndices_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpResultWithIndicesIndices(TNode<JSRegExpResult> p_o, TNode<Object> p_v) {
return StoreJSRegExpResultWithIndicesIndices_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSRegExpResultIndicesGroups(TNode<JSArray> p_o) {
return LoadJSRegExpResultIndicesGroups_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSRegExpResultIndicesGroups(TNode<JSArray> p_o, TNode<Object> p_v) {
return StoreJSRegExpResultIndicesGroups_0(state_, p_o, p_v);}

TNode<NativeContext> TorqueGeneratedExportedMacrosAssembler::LoadJSFinalizationRegistryNativeContext(TNode<JSFinalizationRegistry> p_o) {
return LoadJSFinalizationRegistryNativeContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFinalizationRegistryNativeContext(TNode<JSFinalizationRegistry> p_o, TNode<NativeContext> p_v) {
return StoreJSFinalizationRegistryNativeContext_0(state_, p_o, p_v);}

TNode<JSReceiver> TorqueGeneratedExportedMacrosAssembler::LoadJSFinalizationRegistryCleanup(TNode<JSFinalizationRegistry> p_o) {
return LoadJSFinalizationRegistryCleanup_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFinalizationRegistryCleanup(TNode<JSFinalizationRegistry> p_o, TNode<JSReceiver> p_v) {
return StoreJSFinalizationRegistryCleanup_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSFinalizationRegistryActiveCells(TNode<JSFinalizationRegistry> p_o) {
return LoadJSFinalizationRegistryActiveCells_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFinalizationRegistryActiveCells(TNode<JSFinalizationRegistry> p_o, TNode<HeapObject> p_v) {
return StoreJSFinalizationRegistryActiveCells_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSFinalizationRegistryClearedCells(TNode<JSFinalizationRegistry> p_o) {
return LoadJSFinalizationRegistryClearedCells_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFinalizationRegistryClearedCells(TNode<JSFinalizationRegistry> p_o, TNode<HeapObject> p_v) {
return StoreJSFinalizationRegistryClearedCells_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadJSFinalizationRegistryKeyMap(TNode<JSFinalizationRegistry> p_o) {
return LoadJSFinalizationRegistryKeyMap_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFinalizationRegistryKeyMap(TNode<JSFinalizationRegistry> p_o, TNode<Object> p_v) {
return StoreJSFinalizationRegistryKeyMap_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSFinalizationRegistryNextDirty(TNode<JSFinalizationRegistry> p_o) {
return LoadJSFinalizationRegistryNextDirty_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFinalizationRegistryNextDirty(TNode<JSFinalizationRegistry> p_o, TNode<HeapObject> p_v) {
return StoreJSFinalizationRegistryNextDirty_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadJSFinalizationRegistryFlags(TNode<JSFinalizationRegistry> p_o) {
return LoadJSFinalizationRegistryFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSFinalizationRegistryFlags(TNode<JSFinalizationRegistry> p_o, TNode<Smi> p_v) {
return StoreJSFinalizationRegistryFlags_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadJSWeakRefTarget(TNode<JSWeakRef> p_o) {
return LoadJSWeakRefTarget_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSWeakRefTarget(TNode<JSWeakRef> p_o, TNode<HeapObject> p_v) {
return StoreJSWeakRefTarget_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadArrayBoilerplateDescriptionFlags(TNode<ArrayBoilerplateDescription> p_o) {
return LoadArrayBoilerplateDescriptionFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreArrayBoilerplateDescriptionFlags(TNode<ArrayBoilerplateDescription> p_o, TNode<Smi> p_v) {
return StoreArrayBoilerplateDescriptionFlags_0(state_, p_o, p_v);}

TNode<FixedArrayBase> TorqueGeneratedExportedMacrosAssembler::LoadArrayBoilerplateDescriptionConstantElements(TNode<ArrayBoilerplateDescription> p_o) {
return LoadArrayBoilerplateDescriptionConstantElements_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreArrayBoilerplateDescriptionConstantElements(TNode<ArrayBoilerplateDescription> p_o, TNode<FixedArrayBase> p_v) {
return StoreArrayBoilerplateDescriptionConstantElements_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadRegExpBoilerplateDescriptionData(TNode<RegExpBoilerplateDescription> p_o) {
return LoadRegExpBoilerplateDescriptionData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreRegExpBoilerplateDescriptionData(TNode<RegExpBoilerplateDescription> p_o, TNode<FixedArray> p_v) {
return StoreRegExpBoilerplateDescriptionData_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadRegExpBoilerplateDescriptionSource(TNode<RegExpBoilerplateDescription> p_o) {
return LoadRegExpBoilerplateDescriptionSource_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreRegExpBoilerplateDescriptionSource(TNode<RegExpBoilerplateDescription> p_o, TNode<String> p_v) {
return StoreRegExpBoilerplateDescriptionSource_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadRegExpBoilerplateDescriptionFlags(TNode<RegExpBoilerplateDescription> p_o) {
return LoadRegExpBoilerplateDescriptionFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreRegExpBoilerplateDescriptionFlags(TNode<RegExpBoilerplateDescription> p_o, TNode<Smi> p_v) {
return StoreRegExpBoilerplateDescriptionFlags_0(state_, p_o, p_v);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadMegaDomHandlerAccessor(TNode<MegaDomHandler> p_o) {
return LoadMegaDomHandlerAccessor_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMegaDomHandlerAccessor(TNode<MegaDomHandler> p_o, TNode<MaybeObject> p_v) {
return StoreMegaDomHandlerAccessor_0(state_, p_o, p_v);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadMegaDomHandlerContext(TNode<MegaDomHandler> p_o) {
return LoadMegaDomHandlerContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreMegaDomHandlerContext(TNode<MegaDomHandler> p_o, TNode<MaybeObject> p_v) {
return StoreMegaDomHandlerContext_0(state_, p_o, p_v);}

TNode<Foreign> TorqueGeneratedExportedMacrosAssembler::LoadCallbackTaskCallback(TNode<CallbackTask> p_o) {
return LoadCallbackTaskCallback_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCallbackTaskCallback(TNode<CallbackTask> p_o, TNode<Foreign> p_v) {
return StoreCallbackTaskCallback_0(state_, p_o, p_v);}

TNode<Foreign> TorqueGeneratedExportedMacrosAssembler::LoadCallbackTaskData(TNode<CallbackTask> p_o) {
return LoadCallbackTaskData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCallbackTaskData(TNode<CallbackTask> p_o, TNode<Foreign> p_v) {
return StoreCallbackTaskData_0(state_, p_o, p_v);}

TNode<JSReceiver> TorqueGeneratedExportedMacrosAssembler::LoadCallableTaskCallable(TNode<CallableTask> p_o) {
return LoadCallableTaskCallable_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCallableTaskCallable(TNode<CallableTask> p_o, TNode<JSReceiver> p_v) {
return StoreCallableTaskCallable_0(state_, p_o, p_v);}

TNode<Context> TorqueGeneratedExportedMacrosAssembler::LoadCallableTaskContext(TNode<CallableTask> p_o) {
return LoadCallableTaskContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCallableTaskContext(TNode<CallableTask> p_o, TNode<Context> p_v) {
return StoreCallableTaskContext_0(state_, p_o, p_v);}

TNode<ObjectHashTable> TorqueGeneratedExportedMacrosAssembler::LoadModuleExports(TNode<Module> p_o) {
return LoadModuleExports_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreModuleExports(TNode<Module> p_o, TNode<ObjectHashTable> p_v) {
return StoreModuleExports_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadModuleHash(TNode<Module> p_o) {
return LoadModuleHash_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreModuleHash(TNode<Module> p_o, TNode<Smi> p_v) {
return StoreModuleHash_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadModuleStatus(TNode<Module> p_o) {
return LoadModuleStatus_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreModuleStatus(TNode<Module> p_o, TNode<Smi> p_v) {
return StoreModuleStatus_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadModuleModuleNamespace(TNode<Module> p_o) {
return LoadModuleModuleNamespace_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreModuleModuleNamespace(TNode<Module> p_o, TNode<HeapObject> p_v) {
return StoreModuleModuleNamespace_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadModuleException(TNode<Module> p_o) {
return LoadModuleException_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreModuleException(TNode<Module> p_o, TNode<Object> p_v) {
return StoreModuleException_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadModuleTopLevelCapability(TNode<Module> p_o) {
return LoadModuleTopLevelCapability_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreModuleTopLevelCapability(TNode<Module> p_o, TNode<HeapObject> p_v) {
return StoreModuleTopLevelCapability_0(state_, p_o, p_v);}

TNode<Module> TorqueGeneratedExportedMacrosAssembler::LoadJSModuleNamespaceModule(TNode<JSModuleNamespace> p_o) {
return LoadJSModuleNamespaceModule_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreJSModuleNamespaceModule(TNode<JSModuleNamespace> p_o, TNode<Module> p_v) {
return StoreJSModuleNamespaceModule_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashSetNumberOfElements(TNode<SmallOrderedHashSet> p_o) {
return LoadSmallOrderedHashSetNumberOfElements_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashSetNumberOfElements(TNode<SmallOrderedHashSet> p_o, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashSetNumberOfElements_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashSetNumberOfDeletedElements(TNode<SmallOrderedHashSet> p_o) {
return LoadSmallOrderedHashSetNumberOfDeletedElements_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashSetNumberOfDeletedElements(TNode<SmallOrderedHashSet> p_o, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashSetNumberOfDeletedElements_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashSetNumberOfBuckets(TNode<SmallOrderedHashSet> p_o) {
return LoadSmallOrderedHashSetNumberOfBuckets_0(state_, p_o);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedHashSetPadding(TNode<SmallOrderedHashSet> p_o) {
return FieldSliceSmallOrderedHashSetPadding_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashSetPadding(TNode<SmallOrderedHashSet> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedHashSetPadding_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashSetPadding(TNode<SmallOrderedHashSet> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashSetPadding_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_MutableReference_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedHashSetDataTable(TNode<SmallOrderedHashSet> p_o) {
return FieldSliceSmallOrderedHashSetDataTable_0(state_, p_o);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashSetDataTable(TNode<SmallOrderedHashSet> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedHashSetDataTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashSetDataTable(TNode<SmallOrderedHashSet> p_o, TNode<IntPtrT> p_i, TNode<Object> p_v) {
return StoreSmallOrderedHashSetDataTable_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedHashSetHashTable(TNode<SmallOrderedHashSet> p_o) {
return FieldSliceSmallOrderedHashSetHashTable_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashSetHashTable(TNode<SmallOrderedHashSet> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedHashSetHashTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashSetHashTable(TNode<SmallOrderedHashSet> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashSetHashTable_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedHashSetChainTable(TNode<SmallOrderedHashSet> p_o) {
return FieldSliceSmallOrderedHashSetChainTable_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashSetChainTable(TNode<SmallOrderedHashSet> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedHashSetChainTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashSetChainTable(TNode<SmallOrderedHashSet> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashSetChainTable_0(state_, p_o, p_i, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashMapNumberOfElements(TNode<SmallOrderedHashMap> p_o) {
return LoadSmallOrderedHashMapNumberOfElements_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashMapNumberOfElements(TNode<SmallOrderedHashMap> p_o, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashMapNumberOfElements_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashMapNumberOfDeletedElements(TNode<SmallOrderedHashMap> p_o) {
return LoadSmallOrderedHashMapNumberOfDeletedElements_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashMapNumberOfDeletedElements(TNode<SmallOrderedHashMap> p_o, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashMapNumberOfDeletedElements_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashMapNumberOfBuckets(TNode<SmallOrderedHashMap> p_o) {
return LoadSmallOrderedHashMapNumberOfBuckets_0(state_, p_o);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedHashMapPadding(TNode<SmallOrderedHashMap> p_o) {
return FieldSliceSmallOrderedHashMapPadding_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashMapPadding(TNode<SmallOrderedHashMap> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedHashMapPadding_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashMapPadding(TNode<SmallOrderedHashMap> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashMapPadding_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_HashMapEntry_MutableReference_HashMapEntry_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedHashMapDataTable(TNode<SmallOrderedHashMap> p_o) {
return FieldSliceSmallOrderedHashMapDataTable_0(state_, p_o);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedHashMapHashTable(TNode<SmallOrderedHashMap> p_o) {
return FieldSliceSmallOrderedHashMapHashTable_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashMapHashTable(TNode<SmallOrderedHashMap> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedHashMapHashTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashMapHashTable(TNode<SmallOrderedHashMap> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashMapHashTable_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedHashMapChainTable(TNode<SmallOrderedHashMap> p_o) {
return FieldSliceSmallOrderedHashMapChainTable_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedHashMapChainTable(TNode<SmallOrderedHashMap> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedHashMapChainTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedHashMapChainTable(TNode<SmallOrderedHashMap> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSmallOrderedHashMapChainTable_0(state_, p_o, p_i, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedNameDictionaryHash(TNode<SmallOrderedNameDictionary> p_o) {
return LoadSmallOrderedNameDictionaryHash_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedNameDictionaryHash(TNode<SmallOrderedNameDictionary> p_o, TNode<Int32T> p_v) {
return StoreSmallOrderedNameDictionaryHash_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedNameDictionaryNumberOfElements(TNode<SmallOrderedNameDictionary> p_o) {
return LoadSmallOrderedNameDictionaryNumberOfElements_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedNameDictionaryNumberOfElements(TNode<SmallOrderedNameDictionary> p_o, TNode<Uint8T> p_v) {
return StoreSmallOrderedNameDictionaryNumberOfElements_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedNameDictionaryNumberOfDeletedElements(TNode<SmallOrderedNameDictionary> p_o) {
return LoadSmallOrderedNameDictionaryNumberOfDeletedElements_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedNameDictionaryNumberOfDeletedElements(TNode<SmallOrderedNameDictionary> p_o, TNode<Uint8T> p_v) {
return StoreSmallOrderedNameDictionaryNumberOfDeletedElements_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedNameDictionaryNumberOfBuckets(TNode<SmallOrderedNameDictionary> p_o) {
return LoadSmallOrderedNameDictionaryNumberOfBuckets_0(state_, p_o);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedNameDictionaryPadding1(TNode<SmallOrderedNameDictionary> p_o) {
return FieldSliceSmallOrderedNameDictionaryPadding1_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedNameDictionaryPadding1(TNode<SmallOrderedNameDictionary> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedNameDictionaryPadding1_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedNameDictionaryPadding1(TNode<SmallOrderedNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSmallOrderedNameDictionaryPadding1_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_NameDictionaryEntry_MutableReference_NameDictionaryEntry_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedNameDictionaryDataTable(TNode<SmallOrderedNameDictionary> p_o) {
return FieldSliceSmallOrderedNameDictionaryDataTable_0(state_, p_o);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedNameDictionaryHashTable(TNode<SmallOrderedNameDictionary> p_o) {
return FieldSliceSmallOrderedNameDictionaryHashTable_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedNameDictionaryHashTable(TNode<SmallOrderedNameDictionary> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedNameDictionaryHashTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedNameDictionaryHashTable(TNode<SmallOrderedNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSmallOrderedNameDictionaryHashTable_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSmallOrderedNameDictionaryChainTable(TNode<SmallOrderedNameDictionary> p_o) {
return FieldSliceSmallOrderedNameDictionaryChainTable_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSmallOrderedNameDictionaryChainTable(TNode<SmallOrderedNameDictionary> p_o, TNode<IntPtrT> p_i) {
return LoadSmallOrderedNameDictionaryChainTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmallOrderedNameDictionaryChainTable(TNode<SmallOrderedNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSmallOrderedNameDictionaryChainTable_0(state_, p_o, p_i, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionNext(TNode<PromiseReaction> p_o) {
return LoadPromiseReactionNext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionNext(TNode<PromiseReaction> p_o, TNode<Object> p_v) {
return StorePromiseReactionNext_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionRejectHandler(TNode<PromiseReaction> p_o) {
return LoadPromiseReactionRejectHandler_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionRejectHandler(TNode<PromiseReaction> p_o, TNode<HeapObject> p_v) {
return StorePromiseReactionRejectHandler_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionFulfillHandler(TNode<PromiseReaction> p_o) {
return LoadPromiseReactionFulfillHandler_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionFulfillHandler(TNode<PromiseReaction> p_o, TNode<HeapObject> p_v) {
return StorePromiseReactionFulfillHandler_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionPromiseOrCapability(TNode<PromiseReaction> p_o) {
return LoadPromiseReactionPromiseOrCapability_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionPromiseOrCapability(TNode<PromiseReaction> p_o, TNode<HeapObject> p_v) {
return StorePromiseReactionPromiseOrCapability_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionContinuationPreservedEmbedderData(TNode<PromiseReaction> p_o) {
return LoadPromiseReactionContinuationPreservedEmbedderData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionContinuationPreservedEmbedderData(TNode<PromiseReaction> p_o, TNode<Object> p_v) {
return StorePromiseReactionContinuationPreservedEmbedderData_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionJobTaskArgument(TNode<PromiseReactionJobTask> p_o) {
return LoadPromiseReactionJobTaskArgument_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionJobTaskArgument(TNode<PromiseReactionJobTask> p_o, TNode<Object> p_v) {
return StorePromiseReactionJobTaskArgument_0(state_, p_o, p_v);}

TNode<Context> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionJobTaskContext(TNode<PromiseReactionJobTask> p_o) {
return LoadPromiseReactionJobTaskContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionJobTaskContext(TNode<PromiseReactionJobTask> p_o, TNode<Context> p_v) {
return StorePromiseReactionJobTaskContext_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionJobTaskHandler(TNode<PromiseReactionJobTask> p_o) {
return LoadPromiseReactionJobTaskHandler_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionJobTaskHandler(TNode<PromiseReactionJobTask> p_o, TNode<HeapObject> p_v) {
return StorePromiseReactionJobTaskHandler_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionJobTaskPromiseOrCapability(TNode<PromiseReactionJobTask> p_o) {
return LoadPromiseReactionJobTaskPromiseOrCapability_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionJobTaskPromiseOrCapability(TNode<PromiseReactionJobTask> p_o, TNode<HeapObject> p_v) {
return StorePromiseReactionJobTaskPromiseOrCapability_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPromiseReactionJobTaskContinuationPreservedEmbedderData(TNode<PromiseReactionJobTask> p_o) {
return LoadPromiseReactionJobTaskContinuationPreservedEmbedderData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseReactionJobTaskContinuationPreservedEmbedderData(TNode<PromiseReactionJobTask> p_o, TNode<Object> p_v) {
return StorePromiseReactionJobTaskContinuationPreservedEmbedderData_0(state_, p_o, p_v);}

TNode<Context> TorqueGeneratedExportedMacrosAssembler::LoadPromiseResolveThenableJobTaskContext(TNode<PromiseResolveThenableJobTask> p_o) {
return LoadPromiseResolveThenableJobTaskContext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseResolveThenableJobTaskContext(TNode<PromiseResolveThenableJobTask> p_o, TNode<Context> p_v) {
return StorePromiseResolveThenableJobTaskContext_0(state_, p_o, p_v);}

TNode<JSPromise> TorqueGeneratedExportedMacrosAssembler::LoadPromiseResolveThenableJobTaskPromiseToResolve(TNode<PromiseResolveThenableJobTask> p_o) {
return LoadPromiseResolveThenableJobTaskPromiseToResolve_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseResolveThenableJobTaskPromiseToResolve(TNode<PromiseResolveThenableJobTask> p_o, TNode<JSPromise> p_v) {
return StorePromiseResolveThenableJobTaskPromiseToResolve_0(state_, p_o, p_v);}

TNode<JSReceiver> TorqueGeneratedExportedMacrosAssembler::LoadPromiseResolveThenableJobTaskThenable(TNode<PromiseResolveThenableJobTask> p_o) {
return LoadPromiseResolveThenableJobTaskThenable_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseResolveThenableJobTaskThenable(TNode<PromiseResolveThenableJobTask> p_o, TNode<JSReceiver> p_v) {
return StorePromiseResolveThenableJobTaskThenable_0(state_, p_o, p_v);}

TNode<JSReceiver> TorqueGeneratedExportedMacrosAssembler::LoadPromiseResolveThenableJobTaskThen(TNode<PromiseResolveThenableJobTask> p_o) {
return LoadPromiseResolveThenableJobTaskThen_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePromiseResolveThenableJobTaskThen(TNode<PromiseResolveThenableJobTask> p_o, TNode<JSReceiver> p_v) {
return StorePromiseResolveThenableJobTaskThen_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadPropertyArrayLengthAndHash(TNode<PropertyArray> p_o) {
return LoadPropertyArrayLengthAndHash_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePropertyArrayLengthAndHash(TNode<PropertyArray> p_o, TNode<Smi> p_v) {
return StorePropertyArrayLengthAndHash_0(state_, p_o, p_v);}

TNode<Name> TorqueGeneratedExportedMacrosAssembler::LoadPropertyCellName(TNode<PropertyCell> p_o) {
return LoadPropertyCellName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePropertyCellName(TNode<PropertyCell> p_o, TNode<Name> p_v) {
return StorePropertyCellName_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadPropertyCellPropertyDetailsRaw(TNode<PropertyCell> p_o) {
return LoadPropertyCellPropertyDetailsRaw_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePropertyCellPropertyDetailsRaw(TNode<PropertyCell> p_o, TNode<Smi> p_v) {
return StorePropertyCellPropertyDetailsRaw_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPropertyCellValue(TNode<PropertyCell> p_o) {
return LoadPropertyCellValue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePropertyCellValue(TNode<PropertyCell> p_o, TNode<Object> p_v) {
return StorePropertyCellValue_0(state_, p_o, p_v);}

TNode<WeakFixedArray> TorqueGeneratedExportedMacrosAssembler::LoadPropertyCellDependentCode(TNode<PropertyCell> p_o) {
return LoadPropertyCellDependentCode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePropertyCellDependentCode(TNode<PropertyCell> p_o, TNode<WeakFixedArray> p_v) {
return StorePropertyCellDependentCode_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadPropertyDescriptorObjectFlags(TNode<PropertyDescriptorObject> p_o) {
return LoadPropertyDescriptorObjectFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePropertyDescriptorObjectFlags(TNode<PropertyDescriptorObject> p_o, TNode<Smi> p_v) {
return StorePropertyDescriptorObjectFlags_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPropertyDescriptorObjectValue(TNode<PropertyDescriptorObject> p_o) {
return LoadPropertyDescriptorObjectValue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePropertyDescriptorObjectValue(TNode<PropertyDescriptorObject> p_o, TNode<Object> p_v) {
return StorePropertyDescriptorObjectValue_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPropertyDescriptorObjectGet(TNode<PropertyDescriptorObject> p_o) {
return LoadPropertyDescriptorObjectGet_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePropertyDescriptorObjectGet(TNode<PropertyDescriptorObject> p_o, TNode<Object> p_v) {
return StorePropertyDescriptorObjectGet_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPropertyDescriptorObjectSet(TNode<PropertyDescriptorObject> p_o) {
return LoadPropertyDescriptorObjectSet_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePropertyDescriptorObjectSet(TNode<PropertyDescriptorObject> p_o, TNode<Object> p_v) {
return StorePropertyDescriptorObjectSet_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadPrototypeInfoModuleNamespace(TNode<PrototypeInfo> p_o) {
return LoadPrototypeInfoModuleNamespace_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePrototypeInfoModuleNamespace(TNode<PrototypeInfo> p_o, TNode<HeapObject> p_v) {
return StorePrototypeInfoModuleNamespace_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPrototypeInfoPrototypeUsers(TNode<PrototypeInfo> p_o) {
return LoadPrototypeInfoPrototypeUsers_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePrototypeInfoPrototypeUsers(TNode<PrototypeInfo> p_o, TNode<Object> p_v) {
return StorePrototypeInfoPrototypeUsers_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadPrototypeInfoPrototypeChainEnumCache(TNode<PrototypeInfo> p_o) {
return LoadPrototypeInfoPrototypeChainEnumCache_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePrototypeInfoPrototypeChainEnumCache(TNode<PrototypeInfo> p_o, TNode<Object> p_v) {
return StorePrototypeInfoPrototypeChainEnumCache_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadPrototypeInfoRegistrySlot(TNode<PrototypeInfo> p_o) {
return LoadPrototypeInfoRegistrySlot_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePrototypeInfoRegistrySlot(TNode<PrototypeInfo> p_o, TNode<Smi> p_v) {
return StorePrototypeInfoRegistrySlot_0(state_, p_o, p_v);}

TNode<MaybeObject> TorqueGeneratedExportedMacrosAssembler::LoadPrototypeInfoObjectCreateMap(TNode<PrototypeInfo> p_o) {
return LoadPrototypeInfoObjectCreateMap_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePrototypeInfoObjectCreateMap(TNode<PrototypeInfo> p_o, TNode<MaybeObject> p_v) {
return StorePrototypeInfoObjectCreateMap_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadPrototypeInfoBitField(TNode<PrototypeInfo> p_o) {
return LoadPrototypeInfoBitField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePrototypeInfoBitField(TNode<PrototypeInfo> p_o, TNode<Smi> p_v) {
return StorePrototypeInfoBitField_0(state_, p_o, p_v);}

TNode<PrimitiveHeapObject> TorqueGeneratedExportedMacrosAssembler::LoadScriptSource(TNode<Script> p_o) {
return LoadScriptSource_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptSource(TNode<Script> p_o, TNode<PrimitiveHeapObject> p_v) {
return StoreScriptSource_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadScriptName(TNode<Script> p_o) {
return LoadScriptName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptName(TNode<Script> p_o, TNode<Object> p_v) {
return StoreScriptName_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScriptLineOffset(TNode<Script> p_o) {
return LoadScriptLineOffset_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptLineOffset(TNode<Script> p_o, TNode<Smi> p_v) {
return StoreScriptLineOffset_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScriptColumnOffset(TNode<Script> p_o) {
return LoadScriptColumnOffset_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptColumnOffset(TNode<Script> p_o, TNode<Smi> p_v) {
return StoreScriptColumnOffset_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadScriptContextData(TNode<Script> p_o) {
return LoadScriptContextData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptContextData(TNode<Script> p_o, TNode<Object> p_v) {
return StoreScriptContextData_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScriptScriptType(TNode<Script> p_o) {
return LoadScriptScriptType_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptScriptType(TNode<Script> p_o, TNode<Smi> p_v) {
return StoreScriptScriptType_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadScriptLineEnds(TNode<Script> p_o) {
return LoadScriptLineEnds_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptLineEnds(TNode<Script> p_o, TNode<HeapObject> p_v) {
return StoreScriptLineEnds_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScriptId(TNode<Script> p_o) {
return LoadScriptId_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptId(TNode<Script> p_o, TNode<Smi> p_v) {
return StoreScriptId_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadScriptEvalFromSharedOrWrappedArgumentsOrSfiTable(TNode<Script> p_o) {
return LoadScriptEvalFromSharedOrWrappedArgumentsOrSfiTable_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptEvalFromSharedOrWrappedArgumentsOrSfiTable(TNode<Script> p_o, TNode<HeapObject> p_v) {
return StoreScriptEvalFromSharedOrWrappedArgumentsOrSfiTable_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadScriptEvalFromPosition(TNode<Script> p_o) {
return LoadScriptEvalFromPosition_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptEvalFromPosition(TNode<Script> p_o, TNode<Object> p_v) {
return StoreScriptEvalFromPosition_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadScriptSharedFunctionInfos(TNode<Script> p_o) {
return LoadScriptSharedFunctionInfos_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptSharedFunctionInfos(TNode<Script> p_o, TNode<HeapObject> p_v) {
return StoreScriptSharedFunctionInfos_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadScriptFlags(TNode<Script> p_o) {
return LoadScriptFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptFlags(TNode<Script> p_o, TNode<Smi> p_v) {
return StoreScriptFlags_0(state_, p_o, p_v);}

TNode<PrimitiveHeapObject> TorqueGeneratedExportedMacrosAssembler::LoadScriptSourceUrl(TNode<Script> p_o) {
return LoadScriptSourceUrl_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptSourceUrl(TNode<Script> p_o, TNode<PrimitiveHeapObject> p_v) {
return StoreScriptSourceUrl_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadScriptSourceMappingUrl(TNode<Script> p_o) {
return LoadScriptSourceMappingUrl_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptSourceMappingUrl(TNode<Script> p_o, TNode<Object> p_v) {
return StoreScriptSourceMappingUrl_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadScriptHostDefinedOptions(TNode<Script> p_o) {
return LoadScriptHostDefinedOptions_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreScriptHostDefinedOptions(TNode<Script> p_o, TNode<FixedArray> p_v) {
return StoreScriptHostDefinedOptions_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadPreparseDataDataLength(TNode<PreparseData> p_o) {
return LoadPreparseDataDataLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePreparseDataDataLength(TNode<PreparseData> p_o, TNode<Int32T> p_v) {
return StorePreparseDataDataLength_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadPreparseDataChildrenLength(TNode<PreparseData> p_o) {
return LoadPreparseDataChildrenLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StorePreparseDataChildrenLength(TNode<PreparseData> p_o, TNode<Int32T> p_v) {
return StorePreparseDataChildrenLength_0(state_, p_o, p_v);}

TNode<BytecodeArray> TorqueGeneratedExportedMacrosAssembler::LoadInterpreterDataBytecodeArray(TNode<InterpreterData> p_o) {
return LoadInterpreterDataBytecodeArray_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterpreterDataBytecodeArray(TNode<InterpreterData> p_o, TNode<BytecodeArray> p_v) {
return StoreInterpreterDataBytecodeArray_0(state_, p_o, p_v);}

TNode<Code> TorqueGeneratedExportedMacrosAssembler::LoadInterpreterDataInterpreterTrampoline(TNode<InterpreterData> p_o) {
return LoadInterpreterDataInterpreterTrampoline_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInterpreterDataInterpreterTrampoline(TNode<InterpreterData> p_o, TNode<Code> p_v) {
return StoreInterpreterDataInterpreterTrampoline_0(state_, p_o, p_v);}

TNode<Code> TorqueGeneratedExportedMacrosAssembler::LoadBaselineDataBaselineCode(TNode<BaselineData> p_o) {
return LoadBaselineDataBaselineCode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBaselineDataBaselineCode(TNode<BaselineData> p_o, TNode<Code> p_v) {
return StoreBaselineDataBaselineCode_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadBaselineDataData(TNode<BaselineData> p_o) {
return LoadBaselineDataData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreBaselineDataData(TNode<BaselineData> p_o, TNode<HeapObject> p_v) {
return StoreBaselineDataData_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoFunctionData(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoFunctionData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoFunctionData(TNode<SharedFunctionInfo> p_o, TNode<Object> p_v) {
return StoreSharedFunctionInfoFunctionData_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoNameOrScopeInfo(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoNameOrScopeInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoNameOrScopeInfo(TNode<SharedFunctionInfo> p_o, TNode<Object> p_v) {
return StoreSharedFunctionInfoNameOrScopeInfo_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoOuterScopeInfoOrFeedbackMetadata(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoOuterScopeInfoOrFeedbackMetadata_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoOuterScopeInfoOrFeedbackMetadata(TNode<SharedFunctionInfo> p_o, TNode<HeapObject> p_v) {
return StoreSharedFunctionInfoOuterScopeInfoOrFeedbackMetadata_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoScriptOrDebugInfo(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoScriptOrDebugInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoScriptOrDebugInfo(TNode<SharedFunctionInfo> p_o, TNode<HeapObject> p_v) {
return StoreSharedFunctionInfoScriptOrDebugInfo_0(state_, p_o, p_v);}

TNode<Int16T> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoLength(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoLength(TNode<SharedFunctionInfo> p_o, TNode<Int16T> p_v) {
return StoreSharedFunctionInfoLength_0(state_, p_o, p_v);}

TNode<Uint16T> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoFormalParameterCount(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoFormalParameterCount_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoFormalParameterCount(TNode<SharedFunctionInfo> p_o, TNode<Uint16T> p_v) {
return StoreSharedFunctionInfoFormalParameterCount_0(state_, p_o, p_v);}

TNode<Uint16T> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoFunctionTokenOffset(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoFunctionTokenOffset_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoFunctionTokenOffset(TNode<SharedFunctionInfo> p_o, TNode<Uint16T> p_v) {
return StoreSharedFunctionInfoFunctionTokenOffset_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoExpectedNofProperties(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoExpectedNofProperties_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoExpectedNofProperties(TNode<SharedFunctionInfo> p_o, TNode<Uint8T> p_v) {
return StoreSharedFunctionInfoExpectedNofProperties_0(state_, p_o, p_v);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoFlags2(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoFlags2_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoFlags2(TNode<SharedFunctionInfo> p_o, TNode<Uint8T> p_v) {
return StoreSharedFunctionInfoFlags2_0(state_, p_o, p_v);}

TNode<Uint32T> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoFlags(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoFlags(TNode<SharedFunctionInfo> p_o, TNode<Uint32T> p_v) {
return StoreSharedFunctionInfoFlags_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadSharedFunctionInfoFunctionLiteralId(TNode<SharedFunctionInfo> p_o) {
return LoadSharedFunctionInfoFunctionLiteralId_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSharedFunctionInfoFunctionLiteralId(TNode<SharedFunctionInfo> p_o, TNode<Int32T> p_v) {
return StoreSharedFunctionInfoFunctionLiteralId_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadUncompiledDataInferredName(TNode<UncompiledData> p_o) {
return LoadUncompiledDataInferredName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreUncompiledDataInferredName(TNode<UncompiledData> p_o, TNode<String> p_v) {
return StoreUncompiledDataInferredName_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadUncompiledDataStartPosition(TNode<UncompiledData> p_o) {
return LoadUncompiledDataStartPosition_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreUncompiledDataStartPosition(TNode<UncompiledData> p_o, TNode<Int32T> p_v) {
return StoreUncompiledDataStartPosition_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadUncompiledDataEndPosition(TNode<UncompiledData> p_o) {
return LoadUncompiledDataEndPosition_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreUncompiledDataEndPosition(TNode<UncompiledData> p_o, TNode<Int32T> p_v) {
return StoreUncompiledDataEndPosition_0(state_, p_o, p_v);}

TNode<PreparseData> TorqueGeneratedExportedMacrosAssembler::LoadUncompiledDataWithPreparseDataPreparseData(TNode<UncompiledDataWithPreparseData> p_o) {
return LoadUncompiledDataWithPreparseDataPreparseData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreUncompiledDataWithPreparseDataPreparseData(TNode<UncompiledDataWithPreparseData> p_o, TNode<PreparseData> p_v) {
return StoreUncompiledDataWithPreparseDataPreparseData_0(state_, p_o, p_v);}

TNode<ByteArray> TorqueGeneratedExportedMacrosAssembler::LoadOnHeapBasicBlockProfilerDataBlockIds(TNode<OnHeapBasicBlockProfilerData> p_o) {
return LoadOnHeapBasicBlockProfilerDataBlockIds_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOnHeapBasicBlockProfilerDataBlockIds(TNode<OnHeapBasicBlockProfilerData> p_o, TNode<ByteArray> p_v) {
return StoreOnHeapBasicBlockProfilerDataBlockIds_0(state_, p_o, p_v);}

TNode<ByteArray> TorqueGeneratedExportedMacrosAssembler::LoadOnHeapBasicBlockProfilerDataCounts(TNode<OnHeapBasicBlockProfilerData> p_o) {
return LoadOnHeapBasicBlockProfilerDataCounts_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOnHeapBasicBlockProfilerDataCounts(TNode<OnHeapBasicBlockProfilerData> p_o, TNode<ByteArray> p_v) {
return StoreOnHeapBasicBlockProfilerDataCounts_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadOnHeapBasicBlockProfilerDataName(TNode<OnHeapBasicBlockProfilerData> p_o) {
return LoadOnHeapBasicBlockProfilerDataName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOnHeapBasicBlockProfilerDataName(TNode<OnHeapBasicBlockProfilerData> p_o, TNode<String> p_v) {
return StoreOnHeapBasicBlockProfilerDataName_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadOnHeapBasicBlockProfilerDataSchedule(TNode<OnHeapBasicBlockProfilerData> p_o) {
return LoadOnHeapBasicBlockProfilerDataSchedule_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOnHeapBasicBlockProfilerDataSchedule(TNode<OnHeapBasicBlockProfilerData> p_o, TNode<String> p_v) {
return StoreOnHeapBasicBlockProfilerDataSchedule_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadOnHeapBasicBlockProfilerDataCode(TNode<OnHeapBasicBlockProfilerData> p_o) {
return LoadOnHeapBasicBlockProfilerDataCode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOnHeapBasicBlockProfilerDataCode(TNode<OnHeapBasicBlockProfilerData> p_o, TNode<String> p_v) {
return StoreOnHeapBasicBlockProfilerDataCode_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadOnHeapBasicBlockProfilerDataHash(TNode<OnHeapBasicBlockProfilerData> p_o) {
return LoadOnHeapBasicBlockProfilerDataHash_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreOnHeapBasicBlockProfilerDataHash(TNode<OnHeapBasicBlockProfilerData> p_o, TNode<Smi> p_v) {
return StoreOnHeapBasicBlockProfilerDataHash_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleCode(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleCode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleCode(TNode<SourceTextModule> p_o, TNode<HeapObject> p_v) {
return StoreSourceTextModuleCode_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleRegularExports(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleRegularExports_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleRegularExports(TNode<SourceTextModule> p_o, TNode<FixedArray> p_v) {
return StoreSourceTextModuleRegularExports_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleRegularImports(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleRegularImports_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleRegularImports(TNode<SourceTextModule> p_o, TNode<FixedArray> p_v) {
return StoreSourceTextModuleRegularImports_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleRequestedModules(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleRequestedModules_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleRequestedModules(TNode<SourceTextModule> p_o, TNode<FixedArray> p_v) {
return StoreSourceTextModuleRequestedModules_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleImportMeta(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleImportMeta_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleImportMeta(TNode<SourceTextModule> p_o, TNode<HeapObject> p_v) {
return StoreSourceTextModuleImportMeta_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleCycleRoot(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleCycleRoot_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleCycleRoot(TNode<SourceTextModule> p_o, TNode<HeapObject> p_v) {
return StoreSourceTextModuleCycleRoot_0(state_, p_o, p_v);}

TNode<ArrayList> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleAsyncParentModules(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleAsyncParentModules_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleAsyncParentModules(TNode<SourceTextModule> p_o, TNode<ArrayList> p_v) {
return StoreSourceTextModuleAsyncParentModules_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleDfsIndex(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleDfsIndex_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleDfsIndex(TNode<SourceTextModule> p_o, TNode<Smi> p_v) {
return StoreSourceTextModuleDfsIndex_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleDfsAncestorIndex(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleDfsAncestorIndex_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleDfsAncestorIndex(TNode<SourceTextModule> p_o, TNode<Smi> p_v) {
return StoreSourceTextModuleDfsAncestorIndex_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModulePendingAsyncDependencies(TNode<SourceTextModule> p_o) {
return LoadSourceTextModulePendingAsyncDependencies_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModulePendingAsyncDependencies(TNode<SourceTextModule> p_o, TNode<Smi> p_v) {
return StoreSourceTextModulePendingAsyncDependencies_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleFlags(TNode<SourceTextModule> p_o) {
return LoadSourceTextModuleFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleFlags(TNode<SourceTextModule> p_o, TNode<Smi> p_v) {
return StoreSourceTextModuleFlags_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadModuleRequestSpecifier(TNode<ModuleRequest> p_o) {
return LoadModuleRequestSpecifier_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreModuleRequestSpecifier(TNode<ModuleRequest> p_o, TNode<String> p_v) {
return StoreModuleRequestSpecifier_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadModuleRequestImportAssertions(TNode<ModuleRequest> p_o) {
return LoadModuleRequestImportAssertions_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreModuleRequestImportAssertions(TNode<ModuleRequest> p_o, TNode<FixedArray> p_v) {
return StoreModuleRequestImportAssertions_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadModuleRequestPosition(TNode<ModuleRequest> p_o) {
return LoadModuleRequestPosition_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreModuleRequestPosition(TNode<ModuleRequest> p_o, TNode<Smi> p_v) {
return StoreModuleRequestPosition_0(state_, p_o, p_v);}

TNode<PrimitiveHeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleInfoEntryExportName(TNode<SourceTextModuleInfoEntry> p_o) {
return LoadSourceTextModuleInfoEntryExportName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleInfoEntryExportName(TNode<SourceTextModuleInfoEntry> p_o, TNode<PrimitiveHeapObject> p_v) {
return StoreSourceTextModuleInfoEntryExportName_0(state_, p_o, p_v);}

TNode<PrimitiveHeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleInfoEntryLocalName(TNode<SourceTextModuleInfoEntry> p_o) {
return LoadSourceTextModuleInfoEntryLocalName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleInfoEntryLocalName(TNode<SourceTextModuleInfoEntry> p_o, TNode<PrimitiveHeapObject> p_v) {
return StoreSourceTextModuleInfoEntryLocalName_0(state_, p_o, p_v);}

TNode<PrimitiveHeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleInfoEntryImportName(TNode<SourceTextModuleInfoEntry> p_o) {
return LoadSourceTextModuleInfoEntryImportName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleInfoEntryImportName(TNode<SourceTextModuleInfoEntry> p_o, TNode<PrimitiveHeapObject> p_v) {
return StoreSourceTextModuleInfoEntryImportName_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleInfoEntryModuleRequest(TNode<SourceTextModuleInfoEntry> p_o) {
return LoadSourceTextModuleInfoEntryModuleRequest_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleInfoEntryModuleRequest(TNode<SourceTextModuleInfoEntry> p_o, TNode<Smi> p_v) {
return StoreSourceTextModuleInfoEntryModuleRequest_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleInfoEntryCellIndex(TNode<SourceTextModuleInfoEntry> p_o) {
return LoadSourceTextModuleInfoEntryCellIndex_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleInfoEntryCellIndex(TNode<SourceTextModuleInfoEntry> p_o, TNode<Smi> p_v) {
return StoreSourceTextModuleInfoEntryCellIndex_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleInfoEntryBegPos(TNode<SourceTextModuleInfoEntry> p_o) {
return LoadSourceTextModuleInfoEntryBegPos_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleInfoEntryBegPos(TNode<SourceTextModuleInfoEntry> p_o, TNode<Smi> p_v) {
return StoreSourceTextModuleInfoEntryBegPos_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSourceTextModuleInfoEntryEndPos(TNode<SourceTextModuleInfoEntry> p_o) {
return LoadSourceTextModuleInfoEntryEndPos_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSourceTextModuleInfoEntryEndPos(TNode<SourceTextModuleInfoEntry> p_o, TNode<Smi> p_v) {
return StoreSourceTextModuleInfoEntryEndPos_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadStackFrameInfoReceiverOrInstance(TNode<StackFrameInfo> p_o) {
return LoadStackFrameInfoReceiverOrInstance_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreStackFrameInfoReceiverOrInstance(TNode<StackFrameInfo> p_o, TNode<Object> p_v) {
return StoreStackFrameInfoReceiverOrInstance_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadStackFrameInfoFunction(TNode<StackFrameInfo> p_o) {
return LoadStackFrameInfoFunction_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreStackFrameInfoFunction(TNode<StackFrameInfo> p_o, TNode<Object> p_v) {
return StoreStackFrameInfoFunction_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadStackFrameInfoCodeObject(TNode<StackFrameInfo> p_o) {
return LoadStackFrameInfoCodeObject_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreStackFrameInfoCodeObject(TNode<StackFrameInfo> p_o, TNode<HeapObject> p_v) {
return StoreStackFrameInfoCodeObject_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadStackFrameInfoCodeOffsetOrSourcePosition(TNode<StackFrameInfo> p_o) {
return LoadStackFrameInfoCodeOffsetOrSourcePosition_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreStackFrameInfoCodeOffsetOrSourcePosition(TNode<StackFrameInfo> p_o, TNode<Smi> p_v) {
return StoreStackFrameInfoCodeOffsetOrSourcePosition_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadStackFrameInfoFlags(TNode<StackFrameInfo> p_o) {
return LoadStackFrameInfoFlags_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreStackFrameInfoFlags(TNode<StackFrameInfo> p_o, TNode<Smi> p_v) {
return StoreStackFrameInfoFlags_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadStackFrameInfoParameters(TNode<StackFrameInfo> p_o) {
return LoadStackFrameInfoParameters_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreStackFrameInfoParameters(TNode<StackFrameInfo> p_o, TNode<FixedArray> p_v) {
return StoreStackFrameInfoParameters_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadConsStringFirst(TNode<ConsString> p_o) {
return LoadConsStringFirst_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreConsStringFirst(TNode<ConsString> p_o, TNode<String> p_v) {
return StoreConsStringFirst_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadConsStringSecond(TNode<ConsString> p_o) {
return LoadConsStringSecond_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreConsStringSecond(TNode<ConsString> p_o, TNode<String> p_v) {
return StoreConsStringSecond_0(state_, p_o, p_v);}

TNode<ExternalPointerT> TorqueGeneratedExportedMacrosAssembler::LoadExternalStringResource(TNode<ExternalString> p_o) {
return LoadExternalStringResource_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExternalStringResource(TNode<ExternalString> p_o, TNode<ExternalPointerT> p_v) {
return StoreExternalStringResource_0(state_, p_o, p_v);}

TNode<ExternalPointerT> TorqueGeneratedExportedMacrosAssembler::LoadExternalStringResourceData(TNode<ExternalString> p_o) {
return LoadExternalStringResourceData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExternalStringResourceData(TNode<ExternalString> p_o, TNode<ExternalPointerT> p_v) {
return StoreExternalStringResourceData_0(state_, p_o, p_v);}

TorqueStructSlice_char8_ConstReference_char8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSeqOneByteStringChars(TNode<SeqOneByteString> p_o) {
return FieldSliceSeqOneByteStringChars_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSeqOneByteStringChars(TNode<SeqOneByteString> p_o, TNode<IntPtrT> p_i) {
return LoadSeqOneByteStringChars_0(state_, p_o, p_i);}

TorqueStructSlice_char16_ConstReference_char16_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSeqTwoByteStringChars(TNode<SeqTwoByteString> p_o) {
return FieldSliceSeqTwoByteStringChars_0(state_, p_o);}

TNode<Uint16T> TorqueGeneratedExportedMacrosAssembler::LoadSeqTwoByteStringChars(TNode<SeqTwoByteString> p_o, TNode<IntPtrT> p_i) {
return LoadSeqTwoByteStringChars_0(state_, p_o, p_i);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadSlicedStringParent(TNode<SlicedString> p_o) {
return LoadSlicedStringParent_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSlicedStringParent(TNode<SlicedString> p_o, TNode<String> p_v) {
return StoreSlicedStringParent_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSlicedStringOffset(TNode<SlicedString> p_o) {
return LoadSlicedStringOffset_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSlicedStringOffset(TNode<SlicedString> p_o, TNode<Smi> p_v) {
return StoreSlicedStringOffset_0(state_, p_o, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadThinStringActual(TNode<ThinString> p_o) {
return LoadThinStringActual_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreThinStringActual(TNode<ThinString> p_o, TNode<String> p_v) {
return StoreThinStringActual_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadTuple2Value1(TNode<Tuple2> p_o) {
return LoadTuple2Value1_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreTuple2Value1(TNode<Tuple2> p_o, TNode<Object> p_v) {
return StoreTuple2Value1_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadTuple2Value2(TNode<Tuple2> p_o) {
return LoadTuple2Value2_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreTuple2Value2(TNode<Tuple2> p_o, TNode<Object> p_v) {
return StoreTuple2Value2_0(state_, p_o, p_v);}

TNode<Uint32T> TorqueGeneratedExportedMacrosAssembler::LoadSwissNameDictionaryHash(TNode<SwissNameDictionary> p_o) {
return LoadSwissNameDictionaryHash_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSwissNameDictionaryHash(TNode<SwissNameDictionary> p_o, TNode<Uint32T> p_v) {
return StoreSwissNameDictionaryHash_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadSwissNameDictionaryCapacity(TNode<SwissNameDictionary> p_o) {
return LoadSwissNameDictionaryCapacity_0(state_, p_o);}

TNode<ByteArray> TorqueGeneratedExportedMacrosAssembler::LoadSwissNameDictionaryMetaTable(TNode<SwissNameDictionary> p_o) {
return LoadSwissNameDictionaryMetaTable_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSwissNameDictionaryMetaTable(TNode<SwissNameDictionary> p_o, TNode<ByteArray> p_v) {
return StoreSwissNameDictionaryMetaTable_0(state_, p_o, p_v);}

TorqueStructSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_MutableReference_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSwissNameDictionaryDataTable(TNode<SwissNameDictionary> p_o) {
return FieldSliceSwissNameDictionaryDataTable_0(state_, p_o);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadSwissNameDictionaryDataTable(TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i) {
return LoadSwissNameDictionaryDataTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSwissNameDictionaryDataTable(TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Object> p_v) {
return StoreSwissNameDictionaryDataTable_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSwissNameDictionaryCtrlTable(TNode<SwissNameDictionary> p_o) {
return FieldSliceSwissNameDictionaryCtrlTable_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSwissNameDictionaryCtrlTable(TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i) {
return LoadSwissNameDictionaryCtrlTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSwissNameDictionaryCtrlTable(TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSwissNameDictionaryCtrlTable_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_uint8_MutableReference_uint8_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceSwissNameDictionaryPropertyDetailsTable(TNode<SwissNameDictionary> p_o) {
return FieldSliceSwissNameDictionaryPropertyDetailsTable_0(state_, p_o);}

TNode<Uint8T> TorqueGeneratedExportedMacrosAssembler::LoadSwissNameDictionaryPropertyDetailsTable(TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i) {
return LoadSwissNameDictionaryPropertyDetailsTable_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreSwissNameDictionaryPropertyDetailsTable(TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
return StoreSwissNameDictionaryPropertyDetailsTable_0(state_, p_o, p_i, p_v);}

TNode<String> TorqueGeneratedExportedMacrosAssembler::LoadSyntheticModuleName(TNode<SyntheticModule> p_o) {
return LoadSyntheticModuleName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSyntheticModuleName(TNode<SyntheticModule> p_o, TNode<String> p_v) {
return StoreSyntheticModuleName_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadSyntheticModuleExportNames(TNode<SyntheticModule> p_o) {
return LoadSyntheticModuleExportNames_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSyntheticModuleExportNames(TNode<SyntheticModule> p_o, TNode<FixedArray> p_v) {
return StoreSyntheticModuleExportNames_0(state_, p_o, p_v);}

TNode<Foreign> TorqueGeneratedExportedMacrosAssembler::LoadSyntheticModuleEvaluationSteps(TNode<SyntheticModule> p_o) {
return LoadSyntheticModuleEvaluationSteps_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSyntheticModuleEvaluationSteps(TNode<SyntheticModule> p_o, TNode<Foreign> p_v) {
return StoreSyntheticModuleEvaluationSteps_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadCachedTemplateObjectSlotId(TNode<CachedTemplateObject> p_o) {
return LoadCachedTemplateObjectSlotId_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCachedTemplateObjectSlotId(TNode<CachedTemplateObject> p_o, TNode<Smi> p_v) {
return StoreCachedTemplateObjectSlotId_0(state_, p_o, p_v);}

TNode<JSArray> TorqueGeneratedExportedMacrosAssembler::LoadCachedTemplateObjectTemplateObject(TNode<CachedTemplateObject> p_o) {
return LoadCachedTemplateObjectTemplateObject_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCachedTemplateObjectTemplateObject(TNode<CachedTemplateObject> p_o, TNode<JSArray> p_v) {
return StoreCachedTemplateObjectTemplateObject_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadCachedTemplateObjectNext(TNode<CachedTemplateObject> p_o) {
return LoadCachedTemplateObjectNext_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreCachedTemplateObjectNext(TNode<CachedTemplateObject> p_o, TNode<HeapObject> p_v) {
return StoreCachedTemplateObjectNext_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadTemplateObjectDescriptionRawStrings(TNode<TemplateObjectDescription> p_o) {
return LoadTemplateObjectDescriptionRawStrings_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreTemplateObjectDescriptionRawStrings(TNode<TemplateObjectDescription> p_o, TNode<FixedArray> p_v) {
return StoreTemplateObjectDescriptionRawStrings_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadTemplateObjectDescriptionCookedStrings(TNode<TemplateObjectDescription> p_o) {
return LoadTemplateObjectDescriptionCookedStrings_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreTemplateObjectDescriptionCookedStrings(TNode<TemplateObjectDescription> p_o, TNode<FixedArray> p_v) {
return StoreTemplateObjectDescriptionCookedStrings_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadTemplateInfoTag(TNode<TemplateInfo> p_o) {
return LoadTemplateInfoTag_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreTemplateInfoTag(TNode<TemplateInfo> p_o, TNode<Smi> p_v) {
return StoreTemplateInfoTag_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadTemplateInfoSerialNumber(TNode<TemplateInfo> p_o) {
return LoadTemplateInfoSerialNumber_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreTemplateInfoSerialNumber(TNode<TemplateInfo> p_o, TNode<Smi> p_v) {
return StoreTemplateInfoSerialNumber_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadTemplateInfoNumberOfProperties(TNode<TemplateInfo> p_o) {
return LoadTemplateInfoNumberOfProperties_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreTemplateInfoNumberOfProperties(TNode<TemplateInfo> p_o, TNode<Smi> p_v) {
return StoreTemplateInfoNumberOfProperties_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadTemplateInfoPropertyList(TNode<TemplateInfo> p_o) {
return LoadTemplateInfoPropertyList_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreTemplateInfoPropertyList(TNode<TemplateInfo> p_o, TNode<HeapObject> p_v) {
return StoreTemplateInfoPropertyList_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadTemplateInfoPropertyAccessors(TNode<TemplateInfo> p_o) {
return LoadTemplateInfoPropertyAccessors_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreTemplateInfoPropertyAccessors(TNode<TemplateInfo> p_o, TNode<HeapObject> p_v) {
return StoreTemplateInfoPropertyAccessors_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateRareDataPrototypeTemplate(TNode<FunctionTemplateRareData> p_o) {
return LoadFunctionTemplateRareDataPrototypeTemplate_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateRareDataPrototypeTemplate(TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateRareDataPrototypeTemplate_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateRareDataPrototypeProviderTemplate(TNode<FunctionTemplateRareData> p_o) {
return LoadFunctionTemplateRareDataPrototypeProviderTemplate_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateRareDataPrototypeProviderTemplate(TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateRareDataPrototypeProviderTemplate_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateRareDataParentTemplate(TNode<FunctionTemplateRareData> p_o) {
return LoadFunctionTemplateRareDataParentTemplate_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateRareDataParentTemplate(TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateRareDataParentTemplate_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateRareDataNamedPropertyHandler(TNode<FunctionTemplateRareData> p_o) {
return LoadFunctionTemplateRareDataNamedPropertyHandler_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateRareDataNamedPropertyHandler(TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateRareDataNamedPropertyHandler_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateRareDataIndexedPropertyHandler(TNode<FunctionTemplateRareData> p_o) {
return LoadFunctionTemplateRareDataIndexedPropertyHandler_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateRareDataIndexedPropertyHandler(TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateRareDataIndexedPropertyHandler_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateRareDataInstanceTemplate(TNode<FunctionTemplateRareData> p_o) {
return LoadFunctionTemplateRareDataInstanceTemplate_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateRareDataInstanceTemplate(TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateRareDataInstanceTemplate_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateRareDataInstanceCallHandler(TNode<FunctionTemplateRareData> p_o) {
return LoadFunctionTemplateRareDataInstanceCallHandler_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateRareDataInstanceCallHandler(TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateRareDataInstanceCallHandler_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateRareDataAccessCheckInfo(TNode<FunctionTemplateRareData> p_o) {
return LoadFunctionTemplateRareDataAccessCheckInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateRareDataAccessCheckInfo(TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateRareDataAccessCheckInfo_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateRareDataCFunctionOverloads(TNode<FunctionTemplateRareData> p_o) {
return LoadFunctionTemplateRareDataCFunctionOverloads_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateRareDataCFunctionOverloads(TNode<FunctionTemplateRareData> p_o, TNode<FixedArray> p_v) {
return StoreFunctionTemplateRareDataCFunctionOverloads_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateInfoCallCode(TNode<FunctionTemplateInfo> p_o) {
return LoadFunctionTemplateInfoCallCode_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateInfoCallCode(TNode<FunctionTemplateInfo> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateInfoCallCode_0(state_, p_o, p_v);}

TNode<PrimitiveHeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateInfoClassName(TNode<FunctionTemplateInfo> p_o) {
return LoadFunctionTemplateInfoClassName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateInfoClassName(TNode<FunctionTemplateInfo> p_o, TNode<PrimitiveHeapObject> p_v) {
return StoreFunctionTemplateInfoClassName_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateInfoSignature(TNode<FunctionTemplateInfo> p_o) {
return LoadFunctionTemplateInfoSignature_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateInfoSignature(TNode<FunctionTemplateInfo> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateInfoSignature_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateInfoRareData(TNode<FunctionTemplateInfo> p_o) {
return LoadFunctionTemplateInfoRareData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateInfoRareData(TNode<FunctionTemplateInfo> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateInfoRareData_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateInfoSharedFunctionInfo(TNode<FunctionTemplateInfo> p_o) {
return LoadFunctionTemplateInfoSharedFunctionInfo_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateInfoSharedFunctionInfo(TNode<FunctionTemplateInfo> p_o, TNode<HeapObject> p_v) {
return StoreFunctionTemplateInfoSharedFunctionInfo_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateInfoFlag(TNode<FunctionTemplateInfo> p_o) {
return LoadFunctionTemplateInfoFlag_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateInfoFlag(TNode<FunctionTemplateInfo> p_o, TNode<Smi> p_v) {
return StoreFunctionTemplateInfoFlag_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateInfoLength(TNode<FunctionTemplateInfo> p_o) {
return LoadFunctionTemplateInfoLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateInfoLength(TNode<FunctionTemplateInfo> p_o, TNode<Smi> p_v) {
return StoreFunctionTemplateInfoLength_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateInfoCachedPropertyName(TNode<FunctionTemplateInfo> p_o) {
return LoadFunctionTemplateInfoCachedPropertyName_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateInfoCachedPropertyName(TNode<FunctionTemplateInfo> p_o, TNode<Object> p_v) {
return StoreFunctionTemplateInfoCachedPropertyName_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadFunctionTemplateInfoInstanceType(TNode<FunctionTemplateInfo> p_o) {
return LoadFunctionTemplateInfoInstanceType_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreFunctionTemplateInfoInstanceType(TNode<FunctionTemplateInfo> p_o, TNode<Smi> p_v) {
return StoreFunctionTemplateInfoInstanceType_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadObjectTemplateInfoConstructor(TNode<ObjectTemplateInfo> p_o) {
return LoadObjectTemplateInfoConstructor_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreObjectTemplateInfoConstructor(TNode<ObjectTemplateInfo> p_o, TNode<HeapObject> p_v) {
return StoreObjectTemplateInfoConstructor_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadObjectTemplateInfoData(TNode<ObjectTemplateInfo> p_o) {
return LoadObjectTemplateInfoData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreObjectTemplateInfoData(TNode<ObjectTemplateInfo> p_o, TNode<Smi> p_v) {
return StoreObjectTemplateInfoData_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassA(TNode<InternalClass> p_o) {
return LoadInternalClassA_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassA(TNode<InternalClass> p_o, TNode<Smi> p_v) {
return StoreInternalClassA_0(state_, p_o, p_v);}

TNode<Number> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassB(TNode<InternalClass> p_o) {
return LoadInternalClassB_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassB(TNode<InternalClass> p_o, TNode<Number> p_v) {
return StoreInternalClassB_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSmiPairA(TNode<SmiPair> p_o) {
return LoadSmiPairA_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmiPairA(TNode<SmiPair> p_o, TNode<Smi> p_v) {
return StoreSmiPairA_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSmiPairB(TNode<SmiPair> p_o) {
return LoadSmiPairB_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmiPairB(TNode<SmiPair> p_o, TNode<Smi> p_v) {
return StoreSmiPairB_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSmiBoxValue(TNode<SmiBox> p_o) {
return LoadSmiBoxValue_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmiBoxValue(TNode<SmiBox> p_o, TNode<Smi> p_v) {
return StoreSmiBoxValue_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSmiBoxUnrelated(TNode<SmiBox> p_o) {
return LoadSmiBoxUnrelated_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSmiBoxUnrelated(TNode<SmiBox> p_o, TNode<Smi> p_v) {
return StoreSmiBoxUnrelated_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadExportedSubClassBaseA(TNode<ExportedSubClassBase> p_o) {
return LoadExportedSubClassBaseA_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExportedSubClassBaseA(TNode<ExportedSubClassBase> p_o, TNode<HeapObject> p_v) {
return StoreExportedSubClassBaseA_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadExportedSubClassBaseB(TNode<ExportedSubClassBase> p_o) {
return LoadExportedSubClassBaseB_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExportedSubClassBaseB(TNode<ExportedSubClassBase> p_o, TNode<HeapObject> p_v) {
return StoreExportedSubClassBaseB_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadExportedSubClassCField(TNode<ExportedSubClass> p_o) {
return LoadExportedSubClassCField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExportedSubClassCField(TNode<ExportedSubClass> p_o, TNode<Int32T> p_v) {
return StoreExportedSubClassCField_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadExportedSubClassDField(TNode<ExportedSubClass> p_o) {
return LoadExportedSubClassDField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExportedSubClassDField(TNode<ExportedSubClass> p_o, TNode<Int32T> p_v) {
return StoreExportedSubClassDField_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadExportedSubClassEField(TNode<ExportedSubClass> p_o) {
return LoadExportedSubClassEField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExportedSubClassEField(TNode<ExportedSubClass> p_o, TNode<Smi> p_v) {
return StoreExportedSubClassEField_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassWithSmiElementsData(TNode<InternalClassWithSmiElements> p_o) {
return LoadInternalClassWithSmiElementsData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassWithSmiElementsData(TNode<InternalClassWithSmiElements> p_o, TNode<Smi> p_v) {
return StoreInternalClassWithSmiElementsData_0(state_, p_o, p_v);}

TNode<Oddball> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassWithSmiElementsObject(TNode<InternalClassWithSmiElements> p_o) {
return LoadInternalClassWithSmiElementsObject_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassWithSmiElementsObject(TNode<InternalClassWithSmiElements> p_o, TNode<Oddball> p_v) {
return StoreInternalClassWithSmiElementsObject_0(state_, p_o, p_v);}

TorqueStructSlice_Smi_MutableReference_Smi_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceInternalClassWithSmiElementsEntries(TNode<InternalClassWithSmiElements> p_o) {
return FieldSliceInternalClassWithSmiElementsEntries_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassWithSmiElementsEntries(TNode<InternalClassWithSmiElements> p_o, TNode<IntPtrT> p_i) {
return LoadInternalClassWithSmiElementsEntries_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassWithSmiElementsEntries(TNode<InternalClassWithSmiElements> p_o, TNode<IntPtrT> p_i, TNode<Smi> p_v) {
return StoreInternalClassWithSmiElementsEntries_0(state_, p_o, p_i, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassWithStructElementsDummy1(TNode<InternalClassWithStructElements> p_o) {
return LoadInternalClassWithStructElementsDummy1_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassWithStructElementsDummy1(TNode<InternalClassWithStructElements> p_o, TNode<Int32T> p_v) {
return StoreInternalClassWithStructElementsDummy1_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassWithStructElementsDummy2(TNode<InternalClassWithStructElements> p_o) {
return LoadInternalClassWithStructElementsDummy2_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassWithStructElementsDummy2(TNode<InternalClassWithStructElements> p_o, TNode<Int32T> p_v) {
return StoreInternalClassWithStructElementsDummy2_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassWithStructElementsCount(TNode<InternalClassWithStructElements> p_o) {
return LoadInternalClassWithStructElementsCount_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassWithStructElementsData(TNode<InternalClassWithStructElements> p_o) {
return LoadInternalClassWithStructElementsData_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassWithStructElementsData(TNode<InternalClassWithStructElements> p_o, TNode<Smi> p_v) {
return StoreInternalClassWithStructElementsData_0(state_, p_o, p_v);}

TNode<Object> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassWithStructElementsObject(TNode<InternalClassWithStructElements> p_o) {
return LoadInternalClassWithStructElementsObject_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassWithStructElementsObject(TNode<InternalClassWithStructElements> p_o, TNode<Object> p_v) {
return StoreInternalClassWithStructElementsObject_0(state_, p_o, p_v);}

TorqueStructSlice_Smi_MutableReference_Smi_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceInternalClassWithStructElementsEntries(TNode<InternalClassWithStructElements> p_o) {
return FieldSliceInternalClassWithStructElementsEntries_0(state_, p_o);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadInternalClassWithStructElementsEntries(TNode<InternalClassWithStructElements> p_o, TNode<IntPtrT> p_i) {
return LoadInternalClassWithStructElementsEntries_0(state_, p_o, p_i);}

void TorqueGeneratedExportedMacrosAssembler::StoreInternalClassWithStructElementsEntries(TNode<InternalClassWithStructElements> p_o, TNode<IntPtrT> p_i, TNode<Smi> p_v) {
return StoreInternalClassWithStructElementsEntries_0(state_, p_o, p_i, p_v);}

TorqueStructSlice_InternalClassStructElement_MutableReference_InternalClassStructElement_0 TorqueGeneratedExportedMacrosAssembler::FieldSliceInternalClassWithStructElementsMoreEntries(TNode<InternalClassWithStructElements> p_o) {
return FieldSliceInternalClassWithStructElementsMoreEntries_0(state_, p_o);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadExportedSubClass2XField(TNode<ExportedSubClass2> p_o) {
return LoadExportedSubClass2XField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExportedSubClass2XField(TNode<ExportedSubClass2> p_o, TNode<Int32T> p_v) {
return StoreExportedSubClass2XField_0(state_, p_o, p_v);}

TNode<Int32T> TorqueGeneratedExportedMacrosAssembler::LoadExportedSubClass2YField(TNode<ExportedSubClass2> p_o) {
return LoadExportedSubClass2YField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExportedSubClass2YField(TNode<ExportedSubClass2> p_o, TNode<Int32T> p_v) {
return StoreExportedSubClass2YField_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadExportedSubClass2ZField(TNode<ExportedSubClass2> p_o) {
return LoadExportedSubClass2ZField_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreExportedSubClass2ZField(TNode<ExportedSubClass2> p_o, TNode<Smi> p_v) {
return StoreExportedSubClass2ZField_0(state_, p_o, p_v);}

TNode<JSReceiver> TorqueGeneratedExportedMacrosAssembler::LoadSortStateReceiver(TNode<SortState> p_o) {
return LoadSortStateReceiver_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateReceiver(TNode<SortState> p_o, TNode<JSReceiver> p_v) {
return StoreSortStateReceiver_0(state_, p_o, p_v);}

TNode<Map> TorqueGeneratedExportedMacrosAssembler::LoadSortStateInitialReceiverMap(TNode<SortState> p_o) {
return LoadSortStateInitialReceiverMap_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateInitialReceiverMap(TNode<SortState> p_o, TNode<Map> p_v) {
return StoreSortStateInitialReceiverMap_0(state_, p_o, p_v);}

TNode<Number> TorqueGeneratedExportedMacrosAssembler::LoadSortStateInitialReceiverLength(TNode<SortState> p_o) {
return LoadSortStateInitialReceiverLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateInitialReceiverLength(TNode<SortState> p_o, TNode<Number> p_v) {
return StoreSortStateInitialReceiverLength_0(state_, p_o, p_v);}

TNode<HeapObject> TorqueGeneratedExportedMacrosAssembler::LoadSortStateUserCmpFn(TNode<SortState> p_o) {
return LoadSortStateUserCmpFn_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateUserCmpFn(TNode<SortState> p_o, TNode<HeapObject> p_v) {
return StoreSortStateUserCmpFn_0(state_, p_o, p_v);}

TNode<BuiltinPtr> TorqueGeneratedExportedMacrosAssembler::LoadSortStateSortComparePtr(TNode<SortState> p_o) {
return LoadSortStateSortComparePtr_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateSortComparePtr(TNode<SortState> p_o, TNode<BuiltinPtr> p_v) {
return StoreSortStateSortComparePtr_0(state_, p_o, p_v);}

TNode<BuiltinPtr> TorqueGeneratedExportedMacrosAssembler::LoadSortStateLoadFn(TNode<SortState> p_o) {
return LoadSortStateLoadFn_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateLoadFn(TNode<SortState> p_o, TNode<BuiltinPtr> p_v) {
return StoreSortStateLoadFn_0(state_, p_o, p_v);}

TNode<BuiltinPtr> TorqueGeneratedExportedMacrosAssembler::LoadSortStateStoreFn(TNode<SortState> p_o) {
return LoadSortStateStoreFn_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateStoreFn(TNode<SortState> p_o, TNode<BuiltinPtr> p_v) {
return StoreSortStateStoreFn_0(state_, p_o, p_v);}

TNode<BuiltinPtr> TorqueGeneratedExportedMacrosAssembler::LoadSortStateDeleteFn(TNode<SortState> p_o) {
return LoadSortStateDeleteFn_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateDeleteFn(TNode<SortState> p_o, TNode<BuiltinPtr> p_v) {
return StoreSortStateDeleteFn_0(state_, p_o, p_v);}

TNode<BuiltinPtr> TorqueGeneratedExportedMacrosAssembler::LoadSortStateCanUseSameAccessorFn(TNode<SortState> p_o) {
return LoadSortStateCanUseSameAccessorFn_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateCanUseSameAccessorFn(TNode<SortState> p_o, TNode<BuiltinPtr> p_v) {
return StoreSortStateCanUseSameAccessorFn_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSortStateMinGallop(TNode<SortState> p_o) {
return LoadSortStateMinGallop_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateMinGallop(TNode<SortState> p_o, TNode<Smi> p_v) {
return StoreSortStateMinGallop_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSortStatePendingRunsSize(TNode<SortState> p_o) {
return LoadSortStatePendingRunsSize_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStatePendingRunsSize(TNode<SortState> p_o, TNode<Smi> p_v) {
return StoreSortStatePendingRunsSize_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadSortStatePendingRuns(TNode<SortState> p_o) {
return LoadSortStatePendingRuns_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStatePendingRuns(TNode<SortState> p_o, TNode<FixedArray> p_v) {
return StoreSortStatePendingRuns_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadSortStateWorkArray(TNode<SortState> p_o) {
return LoadSortStateWorkArray_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateWorkArray(TNode<SortState> p_o, TNode<FixedArray> p_v) {
return StoreSortStateWorkArray_0(state_, p_o, p_v);}

TNode<FixedArray> TorqueGeneratedExportedMacrosAssembler::LoadSortStateTempArray(TNode<SortState> p_o) {
return LoadSortStateTempArray_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateTempArray(TNode<SortState> p_o, TNode<FixedArray> p_v) {
return StoreSortStateTempArray_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSortStateSortLength(TNode<SortState> p_o) {
return LoadSortStateSortLength_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateSortLength(TNode<SortState> p_o, TNode<Smi> p_v) {
return StoreSortStateSortLength_0(state_, p_o, p_v);}

TNode<Smi> TorqueGeneratedExportedMacrosAssembler::LoadSortStateNumberOfUndefined(TNode<SortState> p_o) {
return LoadSortStateNumberOfUndefined_0(state_, p_o);}

void TorqueGeneratedExportedMacrosAssembler::StoreSortStateNumberOfUndefined(TNode<SortState> p_o, TNode<Smi> p_v) {
return StoreSortStateNumberOfUndefined_0(state_, p_o, p_v);}

}  // namespace internal
}  // namespace v8
