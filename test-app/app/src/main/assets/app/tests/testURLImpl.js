describe("Test URLImpl ", function () {

it("Test invalid URL parsing", function(){
var exceptionCaught = false;
  try {
        const url = new URLImpl('');
        }catch(e){
        exceptionCaught = true;
        }
        expect(exceptionCaught).toBe(true);
});

it("Test valid URL parsing", function(){
var exceptionCaught = false;
  try {
        const url = new URLImpl('https://google.com');
        }catch(e){
        exceptionCaught = true;
        }
        expect(exceptionCaught).toBe(false);
});


it("Test URL fields", function(){
var exceptionCaught = false;
  const url = new URLImpl('https://google.com');
  expect(url.protocol).toBe('https:');
  expect(url.hostname).toBe('google.com');
});

});