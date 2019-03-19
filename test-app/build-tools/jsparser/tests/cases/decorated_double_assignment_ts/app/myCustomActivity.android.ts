import Bundle = android.os.Bundle;

@JavaProxy('org.nativescript.MyCustomActivity')
export class TestActivity extends android.app.Activity {
    static readonly TEST1: string = "my_test";
    public onCreate(savedInstanceState?: Bundle): void {
        super.onCreate(savedInstanceState);
        console.log(TestActivity.TEST1);
    }
}
