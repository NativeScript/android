package com.tns.sgbtestclasses.inheritance.classes;
import com.tns.sgbtestclasses.inheritance.classes.GenericAbstractClassBase;

import java.util.List;

public class GenericAbstractClassBaseHelper {
    public String workGeneric(GenericAbstractClassBase<List<Object>, Object> arg, int item){
        return  arg.workGeneric(item);
    }
    public String workGeneric(GenericAbstractClassBase<List<Object>, Object> arg, int item, List<Object> collection){
        return arg.workGeneric(item, collection);
    }
    public String workGeneric(GenericAbstractClassBase<List<Object>, Object> arg, int item, List<Object> collection, int number) {
        return arg.workGeneric(item, collection, number);
    }
    public String getGenericItem(GenericAbstractClassBase<List<Object>, Object> arg, Object item){

        return arg.getGenericItem(item).toString();
    }

    public String getGenericCollection(GenericAbstractClassBase<List<Object>, Object> arg, List<Object> collection){
        return arg.getGenericCollection(collection).get(0).toString();
    }
}
