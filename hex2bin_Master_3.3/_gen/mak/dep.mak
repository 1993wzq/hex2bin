define GP_DEP                                                                          
#OBJS_MAIN += $(OUT_OBJS_DIR)/$$(basename $1).o                                        
																					   
-include $(OUT_OBJS_DIR)/$(basename $(subst ../,,$1)).d
																					   
$(OUT_OBJS_DIR)/$$(basename $(subst ../,,$1)).o : $1 $$($(subst .,_,$(subst /,_,$(basename $(subst ../,,$1)))))    
	@mkdir -p $$(@D)                                                                   
																						
ifeq ($(GGSB_CMD_LOG),DETAIL)                                                           
	@echo $$(CC) $$< $$($2) -o $$@                                                      
else                                                                                    
	@echo Compiling $$<                                                                 
endif                                                                                   
																						
	@$$(CC) $$($2) $$< -o $$@ 2>&1                                                      
																					   
endef

ALL_OBJS : $(OBJS_MAIN)

#For BC all srcs
$(eval $(call GP_BUILD,cmdoptios,,FLAG_0))

#For BC all srcs
$(eval $(call GP_BUILD,common,,FLAG_0))

#For BC all srcs
$(eval $(call GP_BUILD,iniconfig,,FLAG_0))

#For BC all srcs
$(eval $(call GP_BUILD,logger,,FLAG_0))

#For BC all srcs
$(eval $(call GP_BUILD,generator,,FLAG_0))

#For BC all srcs
$(eval $(call GP_BUILD,hex,,FLAG_0))

#For BC all srcs
$(eval $(call GP_BUILD,GACmain,,FLAG_0))

#For BC all srcs
$(eval $(call GP_BUILD,testcpp,,FLAG_0))

