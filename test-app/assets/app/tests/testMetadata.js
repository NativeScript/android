describe("Tests metadata", function () {
	
	it("Can_access_protected_static_members_of_protected_interface", function () {
		var accountType = android.provider.ContactsContract.SyncColumns.ACCOUNT_TYPE;
		expect(accountType).toBeTruthy(accountType !== undefined);
	});

});