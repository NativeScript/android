package com.telerik.metadata.dx;

import com.android.dex.ClassDef;
import com.android.dex.Dex;
import com.telerik.metadata.ClassMapProvider;
import com.telerik.metadata.desc.ClassDescriptor;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Consumer;

public class DexFile implements ClassMapProvider {
    private final Dex dex;
    private final String path;
    private final Map<String, ClassDescriptor> classMap;
    private static final String CLASS_EXT = ".dex";

    private DexFile(Dex dex, String path) {
        this.dex = dex;
        this.path = path;
        this.classMap = new HashMap<String, ClassDescriptor>();
    }

    @Override
    public Map<String, ClassDescriptor> getClassMap() {
        return classMap;
    }

    @Override
    public String getPath() {
        return path;
    }

    public Dex getDex() {
        return dex;
    }

    public static DexFile readDex(String path) throws IOException {
        Dex dex = new Dex(new File(path));
        DexFile dexFile = new DexFile(dex, path);

        List<String> typeNames = dex.typeNames();

        for (ClassDef classDef : dex.classDefs()) {
            int cdIdx = classDef.getTypeIndex();
            String typeName = typeNames.get(cdIdx);
            String name = typeName.substring(1, typeName.length()-1).replace('/', '.');
            dexFile.classMap.put(name, new ClassInfo(dexFile, classDef));
        }

        return dexFile;
    }
}
