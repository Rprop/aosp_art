/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_COMPILER_OPTIMIZING_SHARPENING_H_
#define ART_COMPILER_OPTIMIZING_SHARPENING_H_

#include "nodes.h"
#include "optimization.h"

namespace art {

class CodeGenerator;
class DexCompilationUnit;

// Optimization that tries to improve the way we dispatch methods and access types,
// fields, etc. Besides actual method sharpening based on receiver type (for example
// virtual->direct), this includes selecting the best available dispatch for
// invoke-static/-direct based on code generator support.
class HSharpening : public HOptimization {
 public:
  HSharpening(HGraph* graph,
              CodeGenerator* codegen,
              const char* name = kSharpeningPassName)
      : HOptimization(graph, name),
        codegen_(codegen) { }

  bool Run() OVERRIDE;

  static constexpr const char* kSharpeningPassName = "sharpening";

  // Used by Sharpening and InstructionSimplifier.
  static void SharpenInvokeStaticOrDirect(HInvokeStaticOrDirect* invoke, CodeGenerator* codegen);

  // Used by the builder and the inliner.
  static HLoadClass::LoadKind ComputeLoadClassKind(HLoadClass* load_class,
                                                   CodeGenerator* codegen,
                                                   const DexCompilationUnit& dex_compilation_unit)
      REQUIRES_SHARED(Locks::mutator_lock_);

  // Used by the builder.
  static TypeCheckKind ComputeTypeCheckKind(ObjPtr<mirror::Class> klass,
                                            CodeGenerator* codegen,
                                            bool needs_access_check)
      REQUIRES_SHARED(Locks::mutator_lock_);

  // Used by the builder.
  static void ProcessLoadString(HLoadString* load_string,
                                CodeGenerator* codegen,
                                const DexCompilationUnit& dex_compilation_unit,
                                VariableSizedHandleScope* handles);

 private:
  CodeGenerator* codegen_;
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_SHARPENING_H_
