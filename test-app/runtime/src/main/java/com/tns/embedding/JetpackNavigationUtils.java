package com.tns.embedding;

import android.view.View;

import androidx.navigation.NavController;
import androidx.navigation.Navigation;

public class JetpackNavigationUtils {
    public static void navigate(View view, String route) {
        NavController nav = Navigation.findNavController(view);
        nav.navigate(route);
    }
}
