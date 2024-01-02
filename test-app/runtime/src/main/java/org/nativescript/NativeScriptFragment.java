package org.nativescript;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import com.tns.embedding.CallbacksStore;
import com.tns.embedding.EmbeddableFragmentCallbacks;

public class NativeScriptFragment extends Fragment {

    private final EmbeddableFragmentCallbacks callbacks = CallbacksStore.getFragmentCallbacks();

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        callbacks.setFragment(this);
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {
        if (NativeScriptActivity.nativeScriptView == null) {
            NativeScriptActivity.nativeScriptView = callbacks.onCreateView(inflater, container, savedInstanceState);
        }

        FrameLayout wrapper = new FrameLayout(requireContext());
        wrapper.addView(NativeScriptActivity.nativeScriptView);
        return wrapper;
    }

    @Override
    public void onResume() {
        super.onResume();
        callbacks.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
        callbacks.onPause();
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        if (NativeScriptActivity.nativeScriptView != null && NativeScriptActivity.nativeScriptView.getParent() != null) {
            ((ViewGroup) NativeScriptActivity.nativeScriptView.getParent()).removeView(NativeScriptActivity.nativeScriptView);
        }
    }
}
