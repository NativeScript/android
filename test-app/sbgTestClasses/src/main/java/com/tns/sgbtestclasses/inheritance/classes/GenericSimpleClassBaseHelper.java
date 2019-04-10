package com.tns.sgbtestclasses.inheritance.classes;
import com.tns.sgbtestclasses.inheritance.classes.GenericSimpleClassBase;

import java.util.List;

public class GenericSimpleClassBaseHelper {
    public String workGeneric(GenericSimpleClassBase<Object, List<Object>> arg, Object item){
        return  arg.workGeneric(item);
    }
    public String workGeneric(GenericSimpleClassBase<Object, List<Object>> arg, Object item, List<Object> collection){
        return arg.workGeneric(item, collection);
    }
    public String workGeneric(GenericSimpleClassBase<Object, List<Object>> arg, Object item, List<Object> collection, int number) {
        return arg.workGeneric(item, collection, number);
    }
    public String getGenericItem(GenericSimpleClassBase<Object, List<Object>> arg, Object item){

        return arg.getGenericItem(item).toString();
    }

    public String getGenericCollection(GenericSimpleClassBase<Object, List<Object>> arg, List<Object> collection){
        return arg.getGenericCollection(collection).get(0).toString();
    }
}
