describe("Tests primitive type conversion", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("convert_char", function () {
		
		var logged =  null;
		
		var logger = new com.tns.tests.CharConversionTest.Logger({
		    print: function(value) {
		        logged = value;
		    }
		});

		com.tns.tests.CharConversionTest.printWithChar(logger, char('x'));
		
		expect(logged).toBe('x');
	});
	

});
describe("Tests char conversion of non-ASCII values", function () {

	var T = com.tns.tests.CharConversionTest;

	// U+00E9, U+20AC, U+4E2D, a lone high surrogate, an ASCII control, ASCII.
	var samples = ["é", "€", "中", "\ud83d", "\u0007", "x"];

	function codeUnits(javaIntArray) {
		var result = [];
		for (var i = 0; i < javaIntArray.length; i++) {
			result.push(javaIntArray[i]);
		}
		return result;
	}

	function expectedUnits(values) {
		return values.map(function (s) { return s.charCodeAt(0); });
	}

	it("char_conversion_char_cast_argument_keeps_the_utf16_code_unit", function () {
		samples.forEach(function (s) {
			expect(T.codeUnitOf(char(s))).toBe(s.charCodeAt(0));
		});
	});

	it("char_conversion_char_return_value_round_trips", function () {
		samples.forEach(function (s) {
			expect(T.echo(char(s))).toBe(s);
		});
	});

	it("char_conversion_js_array_to_char_array_argument", function () {
		expect(codeUnits(T.codeUnitsOf(samples))).toEqual(expectedUnits(samples));
	});

	it("char_conversion_char_array_element_get", function () {
		var arr = T.charsOf(samples.join(""));
		for (var i = 0; i < samples.length; i++) {
			expect(arr[i]).toBe(samples[i]);
		}
	});

	it("char_conversion_char_array_element_set", function () {
		var arr = T.charsOf("aaaaaa");
		for (var i = 0; i < samples.length; i++) {
			arr[i] = samples[i];
		}
		expect(codeUnits(T.codeUnitsOf(arr))).toEqual(expectedUnits(samples));
		for (var j = 0; j < samples.length; j++) {
			expect(arr[j]).toBe(samples[j]);
		}
	});

	it("char_conversion_element_set_takes_first_code_unit_or_nul", function () {
		var arr = T.charsOf("aaa");
		arr[0] = "€b";
		arr[1] = "";
		arr[2] = "😀";
		expect(codeUnits(T.codeUnitsOf(arr))).toEqual([0x20ac, 0, 0xd83d]);
	});

	it("char_conversion_char_field_set_and_get", function () {
		var t = new T();
		samples.forEach(function (s) {
			t.charField = s;
			expect(t.charFieldCodeUnit()).toBe(s.charCodeAt(0));
			expect(t.charField).toBe(s);

			T.staticCharField = s;
			expect(T.staticCharFieldCodeUnit()).toBe(s.charCodeAt(0));
			expect(T.staticCharField).toBe(s);
		});
	});

	it("char_conversion_char_field_set_takes_first_code_unit_or_nul", function () {
		var t = new T();
		t.charField = "€b";
		expect(t.charFieldCodeUnit()).toBe(0x20ac);
		t.charField = "";
		expect(t.charFieldCodeUnit()).toBe(0);
	});

	it("char_conversion_char_cast_returned_from_js_callback", function () {
		var seen = [];
		var callback = new com.tns.tests.CharConversionTest.CharEcho({
			echo: function (c) {
				seen.push(c);
				return char(c);
			}
		});
		samples.forEach(function (s) {
			expect(T.codeUnitFromCallback(callback, char(s))).toBe(s.charCodeAt(0));
		});
		expect(seen).toEqual(samples);
	});

	it("char_conversion_plain_string_argument_takes_first_code_unit_or_nul", function () {
		samples.forEach(function (s) {
			expect(T.codeUnitOf(s)).toBe(s.charCodeAt(0));
			expect(T.echo(s)).toBe(s);
			expect(T.codeUnitOf(new String(s))).toBe(s.charCodeAt(0));
		});
		expect(T.codeUnitOf("\u20acb")).toBe(0x20ac);
		expect(T.codeUnitOf("")).toBe(0);
	});

	it("boolean_field_get_returns_the_java_value", function () {
		var t = new T();
		expect(t.booleanField).toBe(false);
		t.booleanField = true;
		expect(t.booleanField).toBe(true);

		T.staticBooleanField = true;
		expect(T.staticBooleanField).toBe(true);
		T.staticBooleanField = false;
		expect(T.staticBooleanField).toBe(false);
	});
});
