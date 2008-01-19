------------------------------------------------------------------------------
--                           G E L A   A S I S                              --
--       ASIS implementation for Gela project, a portable Ada compiler      --
--                     http://www.ten15.org/wiki/Ada                        --
--                     - - - - - - - - - - - - - - -                        --
--            Read copyright and license at the end of this file            --
------------------------------------------------------------------------------
--  $TenDRA$
--  Purpose:
--  Procedural wrapper over Object-Oriented ASIS implementation

with Asis.Errors;             use Asis.Errors;
with Asis.Exceptions;
with Asis.Implementation;

package body Asis.Compilation_Units.Relations is

   -----------------------
   -- Elaboration_Order --
   -----------------------

   function Elaboration_Order
     (Compilation_Units : in Asis.Compilation_Unit_List;
      The_Context       : in Asis.Context)
      return Relationship
   is
   begin
      Asis.Implementation.Set_Status
        (Not_Implemented_Error, "Elaboration_Order not implemented");
      raise Asis.Exceptions.ASIS_Failed;

      return Elaboration_Order (Compilation_Units, The_Context);
   end Elaboration_Order;

   -------------------------------
   -- Semantic_Dependence_Order --
   -------------------------------

   function Semantic_Dependence_Order
     (Compilation_Units : in Asis.Compilation_Unit_List;
      Dependent_Units   : in Asis.Compilation_Unit_List;
      The_Context       : in Asis.Context;
      Relation          : in Asis.Relation_Kinds)
      return Relationship
   is
   begin
      Asis.Implementation.Set_Status
        (Not_Implemented_Error, "Semantic_Dependence_Order not implemented");
      raise Asis.Exceptions.ASIS_Failed;

      return Semantic_Dependence_Order
        (Compilation_Units, Dependent_Units, The_Context, Relation);
   end Semantic_Dependence_Order;

end Asis.Compilation_Units.Relations;



------------------------------------------------------------------------------
--  Copyright (c) 2006, Maxim Reznik
--  All rights reserved.
--
--  Redistribution and use in source and binary forms, with or without
--  modification, are permitted provided that the following conditions are met:
--
--     * Redistributions of source code must retain the above copyright notice,
--     * this list of conditions and the following disclaimer.
--     * Redistributions in binary form must reproduce the above copyright
--     * notice, this list of conditions and the following disclaimer in the
--     * documentation and/or other materials provided with the distribution.
--
--  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
--  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
--  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
--  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
--  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
--  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
--  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
--  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
--  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
--  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
--  POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------