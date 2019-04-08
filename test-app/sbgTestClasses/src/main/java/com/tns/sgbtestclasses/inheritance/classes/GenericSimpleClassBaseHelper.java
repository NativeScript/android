package com.tns.sgbtestclasses.inheritance.classes;
import com.tns.sgbtestclasses.inheritance.classes.GenericSimpleClassBase;

import java.util.List;

public class GenericSimpleClassBaseHelper {
    public String work(GenericSimpleClassBase<Object, List<Object>> arg, Object item){
        return  arg.workGeneric(item);
    }
    public String work(GenericSimpleClassBase<Object, List<Object>> arg, Object item, List<Object> collection){
        return arg.workGeneric(item, collection);
    }
    public String work(GenericSimpleClassBase<Object, List<Object>> arg, Object item, List<Object> collection, int number) {
        return arg.workGeneric(item, collection, number);
    }
    public String getString(GenericSimpleClassBase<Object, List<Object>> arg, Object item){

        return arg.getGenericItem(item).toString();
    }

    public String getString(GenericSimpleClassBase<Object, List<Object>> arg, List<Object> collection){
        return arg.getGenericCollection(collection).get(0).toString();
    }
}
