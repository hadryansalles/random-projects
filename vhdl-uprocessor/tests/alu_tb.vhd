----------------------------------------------------------------
-- file: alu_tb.vhd
-- authors: Ricky Habegger e Hadryan Salles
-- date: 08/13/2020
-- brief: ALU testbench
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

entity alu_tb is
end;

architecture a_alu_tb of alu_tb is
    -- alu interface
    component alu
        port (
            in0, in1 : in unsigned(15 downto 0); 
            sel      : in unsigned(1 downto 0);
            CY       : out std_logic; -- '1' if a borrow occurs from MSB; otherwise, '0'.
            OV       : out std_logic; -- '1' if overflow occurs; otherwise, '0'.
            S        : out std_logic; -- '1' if the operation result is negative; otherwise, '0'.
            Z        : out std_logic; -- '1' if the operation result is '0'; otherwise, '0'
            data_out : out unsigned(15 downto 0);
            higher_data_out : out unsigned(15 downto 0)
        );
    end component;
    signal in0, in1, data_out, higher_data_out : unsigned(15 downto 0) := "0000000000000000";
    signal CY, OV, S, Z : std_logic := '0';
    signal sel : unsigned(1 downto 0) := "00";
begin
    uut : alu port map (
        in0 => in0,
        in1 => in1,
        sel => sel,
        CY => CY,
        OV => OV,
        S => S,
        Z => Z,
        data_out => data_out
    );
    process
    begin
        -- ========================= SUB ========================= -
        -- 0x793f + 0x7275 -> 0xebb4 positive + positive with overflow
        in0 <= "0111100100111111";
        in1 <= "0111001001110101";
        sel <= "00";
        wait for 50 ns;
        -- 0x095a + 0x26d2 -> 0x302c positive + positive 
        in0 <= "0000100101011010";
        in1 <= "0010011011010010";
        sel <= "00";
        wait for 50 ns;
        -- 0x4db2 + 0xacaf -> 0xfa61 positive + negative result negative
        in0 <= "0100110110110010";
        in1 <= "1010110010101111";
        sel <= "00";
        wait for 50 ns;
        -- 0x7d62 + 0xf073 -> 0x6dd5 positive + negative result positive
        in0 <= "0111110101100010";
        in1 <= "1111000001110011";
        sel <= "00";
        wait for 50 ns;
        -- 0xeffe + 0x0405 -> 0xf403 negative + positive resultuado negative
        in0 <= "1110111111111110";
        in1 <= "0000010000000101";
        sel <= "00";
        wait for 50 ns;
        -- 0xdba0 + 0x6d7d -> 0x491d negative + positive result positive
        in0 <= "1101101110100000";
        in1 <= "0110110101111101";
        sel <= "00";
        wait for 50 ns;
        -- 0xc234 + 0xb009 -> 0x723d negative + negative with overflow
        in0 <= "1100001000110100";
        in1 <= "1011000000001001";
        sel <= "00";
        wait for 50 ns;
        -- 0xf092 + 0xecdb -> 0xdd6d negative + negative  
        in0 <= "1111000010010010";
        in1 <= "1110110011011011";
        sel <= "00";
        wait for 50 ns;

        -- ========================= SUB ========================= -
        -- 0x4db2 - 0x5351 -> 0xfa61 positive - positive result negative
        in0 <= "0100110110110010";
        in1 <= "0101001101010001";
        sel <= "01";
        wait for 50 ns;
        -- 0x7d62 - 0x0f8d -> 0x6dd5 positive - positive result positive
        in0 <= "0111110101100010";
        in1 <= "0000111110001101";
        sel <= "01";
        wait for 50 ns;
        -- 0x793f - 0x8d8b -> 0xebb4 positive - negative with overflow
        in0 <= "0111100100111111";
        in1 <= "1000110110001011";
        sel <= "01";
        wait for 50 ns;
        -- 0x095a - 0xd92e -> 0x302c positive - negative 
        in0 <= "0000100101011010";
        in1 <= "1101100100101110";
        sel <= "01";
        wait for 50 ns;
        -- 0xc234 - 0x4ff7 -> 0x723d negative - positive with overflow
        in0 <= "1100001000110100";
        in1 <= "0100111111110111";
        sel <= "01";
        wait for 50 ns;
        -- 0xf092 - 0x1325 -> 0xdd6d negative - positive  
        in0 <= "1111000010010010";
        in1 <= "0001001100100101";
        sel <= "01";
        wait for 50 ns;
        -- 0xeffe - 0xfbfb -> 0xf403 negative - negative result negative
        in0 <= "1110111111111110";
        in1 <= "1111101111111011";
        sel <= "01";
        wait for 50 ns;
        -- 0xdba0 - 0x9283 -> 0x491d negative - negative result positive
        in0 <= "1101101110100000";
        in1 <= "1001001010000011";
        sel <= "01";
        wait for 50 ns;

        -- ========================= MUL ========================= -
        -- 100 * 0 = 0 | positive * zero
        in0 <= "0000000001100100";
        in1 <= "0000000000000000";
        sel <= "10";
        wait for 50 ns;
        -- -20 * 0 = 0 | negative * zero
        in0 <= "1111111111101100";
        in1 <= "0000000000000000";
        sel <= "10";
        wait for 50 ns;
        -- 100 * 20 = 2000 | positive * positive
        in0 <= "0000000001100100";
        in1 <= "0000000000010100";
        sel <= "10";
        wait for 50 ns;
        -- 100 * (-20) = 2000 | positive * negative
        in0 <= "0000000001100100";
        in1 <= "1111111111101100";
        sel <= "10";
        wait for 50 ns;
        -- -10 * -20 = 200 | negative * negative
        in0 <= "1111111111110110";
        in1 <= "1111111111101100";
        sel <= "10";
        wait for 50 ns;
        -- 100 * 20000 = 2000000 | positive with overflow
        in0 <= "0000000001100100";
        in1 <= "0100111000100000";
        sel <= "10";
        wait for 50 ns;
        -- 10000 * (-20) = 200000 | negative with overflow
        in0 <= "0010011100010000";
        in1 <= "1111111111101100";
        sel <= "10";
        wait for 50 ns;

        -- ===================== NOT ===================== --
        sel <= "11";
        -- !1 = -2
        in0 <= "0000000000000001";
        wait for 50 ns;
        -- !0 = -1 
        in0 <= "0000000000000000";
        wait for 50 ns;
        -- !2 = -3
        in0 <= "0000000000000010";
        wait for 50 ns;
        -- !-1 = 0
        in0 <= "1111111111111111";
        wait for 50 ns;
        -- !-2 = 1
        in0 <= "1111111111111110";
        wait for 50 ns;
        wait;
    end process;
end architecture;
